/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#include "co_routine.h"
#include "co_routine_inner.h"
#include "co_epoll.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>

#include <poll.h>
#include <sys/time.h>
#include <errno.h>

#include <assert.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>

extern "C"
{
	extern void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");
};
using namespace std;
stCoRoutine_t *GetCurrCo( stCoRoutineEnv_t *env );
struct stCoEpoll_t;

struct stCoRoutineEnv_t
{
	stCoRoutine_t *pCallStack[ 128 ]; //一个进程里维护128个协程(但协程是可以嵌套的),初始时 pCallStack[0] 是栈顶,iCallStackSize = 1
	int iCallStackSize; //每个协程的栈大小
	stCoEpoll_t *pEpoll; //所有协程共用的epoll实例

	//for copy stack log lastco and nextco
	stCoRoutine_t* pending_co;
	stCoRoutine_t* occupy_co;
};
//int socket(int domain, int type, int protocol);
void co_log_err( const char *fmt,... )
{
}

/* rdtscp 寄存器用于读取tsc(time stamp counter)寄存器的值,TSC寄存器在每个时钟信号到来时加1.
其数值递增与CPU主频相关,主频1MHz的处理器,一秒后TSC寄存器的值增加100万.
不用担心值会溢出,TSC寄存器有64位,即使是天河一号超级计算机,主频3GHz,跑满2^64值要199年
rdtscp 高32位结果存放在edx寄存器中,低32位结果存放在eax寄存器
*/
#if defined( __LIBCO_RDTSCP__) 
static unsigned long long counter(void)
{
	register uint32_t lo, hi;
	register unsigned long long o;
	__asm__ __volatile__ (
			"rdtscp" : "=a"(lo), "=d"(hi)::"%rcx"
			);
	o = hi;
	o <<= 32;//高32位存放的是hi(edx中读取的值)
	return (o | lo);//把低32位结果(eax中读出)赋值到变量o

}
static unsigned long long getCpuKhz() //获取CPU频率
{
	FILE *fp = fopen("/proc/cpuinfo","r"); // 我们可以使用cat /proc/cpuinfo 看看里面的CPU动态信息
	if(!fp) return 1;
	char buf[4096] = {0};
	fread(buf,1,sizeof(buf),fp);
	fclose(fp);

	char *lp = strstr(buf,"cpu MHz"); // /proc/cpuinfo中有一行是 "cpu MHz: value"这里要将value提取
	if(!lp) return 1;
	lp += strlen("cpu MHz");
	while(*lp == ' ' || *lp == '\t' || *lp == ':')
	{
		++lp;
	}

	double mhz = atof(lp);
	unsigned long long u = (unsigned long long)(mhz * 1000);
	return u;
}
#endif

static unsigned long long GetTickMS() //返回当前时间的时间戳(单位ms)
{
#if defined( __LIBCO_RDTSCP__) 
	static uint32_t khz = getCpuKhz();
	return counter() / khz; //tsc 寄存器 counter 除以CPU主频,得到一个时间戳
#else // 如果无法使用 tsc 寄存器计时,就用 gettimeofday 函数获取时间戳
	struct timeval now = { 0 };
	gettimeofday( &now,NULL );
	unsigned long long u = now.tv_sec;
	u *= 1000;
	u += now.tv_usec / 1000;
	return u;
#endif
}

/* no longer use
static pid_t GetPid()
{
    static __thread pid_t pid = 0;
    static __thread pid_t tid = 0;
    if( !pid || !tid || pid != getpid() )
    {
        pid = getpid();
#if defined( __APPLE__ )
		tid = syscall( SYS_gettid );
		if( -1 == (long)tid )
		{
			tid = pid;
		}
#elif defined( __FreeBSD__ )
		syscall(SYS_thr_self, &tid);
		if( tid < 0 )
		{
			tid = pid;
		}
#else 
        tid = syscall( __NR_gettid );
#endif

    }
    return tid;

}
static pid_t GetPid()
{
	char **p = (char**)pthread_self();
	return p ? *(pid_t*)(p + 18) : getpid();
}
*/
template <class T,class TLink>
void RemoveFromLink(T *ap)
{
	TLink *lst = ap->pLink;
	if(!lst) return ;
	assert( lst->head && lst->tail );

	if( ap == lst->head )
	{
		lst->head = ap->pNext;
		if(lst->head)
		{
			lst->head->pPrev = NULL;
		}
	}
	else
	{
		if(ap->pPrev)
		{
			ap->pPrev->pNext = ap->pNext;
		}
	}

	if( ap == lst->tail )
	{
		lst->tail = ap->pPrev;
		if(lst->tail)
		{
			lst->tail->pNext = NULL;
		}
	}
	else
	{
		ap->pNext->pPrev = ap->pPrev; // 将ap节点的下个节点和ap节点的前个节点连起来,相当于ap节点从链表中移除了
	}

	ap->pPrev = ap->pNext = NULL;
	ap->pLink = NULL;
}

template <class TNode,class TLink>
void inline AddTail(TLink*apLink,TNode *ap)
{
	if( ap->pLink )
	{
		return ;
	}
	if(apLink->tail)
	{
		apLink->tail->pNext = (TNode*)ap;
		ap->pNext = NULL;
		ap->pPrev = apLink->tail;
		apLink->tail = ap;
	}
	else //apLink 一个元素都没有
	{
		apLink->head = apLink->tail = ap;
		ap->pNext = ap->pPrev = NULL;
	}
	ap->pLink = apLink;
}
template <class TNode,class TLink>
void inline PopHead( TLink*apLink )
{
	if( !apLink->head ) 
	{
		return ;
	}
	TNode *lp = apLink->head;
	if( apLink->head == apLink->tail )
	{
		apLink->head = apLink->tail = NULL;
	}
	else
	{
		apLink->head = apLink->head->pNext;
	}

	lp->pPrev = lp->pNext = NULL;
	lp->pLink = NULL;

	if( apLink->head )
	{
		apLink->head->pPrev = NULL;
	}
}

template <class TNode,class TLink>
void inline Join( TLink*apLink,TLink *apOther )
{
	//printf("apOther %p\n",apOther);
	if( !apOther->head )
	{
		return ;
	}
	TNode *lp = apOther->head;
	while( lp )
	{
		lp->pLink = apLink;
		lp = lp->pNext;
	}
	lp = apOther->head;
	if(apLink->tail) //将apLink 和apOther 两个双向链表连接起来
	{
		apLink->tail->pNext = (TNode*)lp;
		lp->pPrev = apLink->tail;
		apLink->tail = apOther->tail; // apLink被更新,它指向了连接后的双向链表
	}
	else //如果apLink是个空链表,则apLink最后即为apOther
	{
		apLink->head = apOther->head;
		apLink->tail = apOther->tail;
	}

	apOther->head = apOther->tail = NULL; //apOther的元素已经都交给apLink了(数据可在apLink中找到,所以不用担心内存泄漏),apOther被置空
}

/////////////////for copy stack //////////////////////////
stStackMem_t* co_alloc_stackmem(unsigned int stack_size)
{
	stStackMem_t* stack_mem = (stStackMem_t*)malloc(sizeof(stStackMem_t));
	stack_mem->occupy_co= NULL;
	stack_mem->stack_size = stack_size;
	stack_mem->stack_buffer = (char*)malloc(stack_size);
	stack_mem->stack_bp = stack_mem->stack_buffer + stack_size;
	return stack_mem;
}

stShareStack_t* co_alloc_sharestack(int count, int stack_size)
{
	stShareStack_t* share_stack = (stShareStack_t*)malloc(sizeof(stShareStack_t));
	share_stack->alloc_idx = 0;
	share_stack->stack_size = stack_size;

	//alloc stack array
	share_stack->count = count;
	stStackMem_t** stack_array = (stStackMem_t**)calloc(count, sizeof(stStackMem_t*));
	for (int i = 0; i < count; i++)
	{
		stack_array[i] = co_alloc_stackmem(stack_size);
	}
	share_stack->stack_array = stack_array;
	return share_stack;
}

static stStackMem_t* co_get_stackmem(stShareStack_t* share_stack)
{
	if (!share_stack)
	{
		return NULL;
	}
	int idx = share_stack->alloc_idx % share_stack->count;
	share_stack->alloc_idx++;

	return share_stack->stack_array[idx];
}


// ----------------------------------------------------------------------------
struct stTimeoutItemLink_t;
struct stTimeoutItem_t;
struct stCoEpoll_t
{
	int iEpollFd;
	static const int _EPOLL_SIZE = 1024 * 10;

	struct stTimeout_t *pTimeout; //时间轮结构体

	struct stTimeoutItemLink_t *pstTimeoutList; // timeout链表

	struct stTimeoutItemLink_t *pstActiveList; //epoll就绪事件和时间轮超时事件构成的链表,eventloop处理完所有事件后该链表置空

	co_epoll_res *result; // 对 Epoll 事件数据结构的封装，也就是每次触发 Epoll 事件时的返回结果,里面封装了 epoll_event 数组

};
typedef void (*OnPreparePfn_t)( stTimeoutItem_t *,struct epoll_event &ev, stTimeoutItemLink_t *active );
typedef void (*OnProcessPfn_t)( stTimeoutItem_t *);
struct stTimeoutItem_t //定时器结构体,封装了定时器任务
{

	enum
	{
		eMaxTimeout = 40 * 1000 //40s
	};
	stTimeoutItem_t *pPrev; //双向链表
	stTimeoutItem_t *pNext;
	stTimeoutItemLink_t *pLink;

	unsigned long long ullExpireTime; // 超时时间的时间戳
  // 以下两个处理函数对象是互斥执行的
	OnPreparePfn_t pfnPrepare;
	OnProcessPfn_t pfnProcess;

	void *pArg; // routine 
	bool bTimeout; // 是否超时,超时置1
};
struct stTimeoutItemLink_t // 结构很简单,只有首尾两个成员
{
	stTimeoutItem_t *head;
	stTimeoutItem_t *tail;

};
struct stTimeout_t
{
	stTimeoutItemLink_t *pItems; //链表数组(每个元素都是一个链表),表示时间轮盘,数组中每个链表元素对应一个槽
	int iItemSize; // 时间轮尺寸 (槽数)

	unsigned long long ullStart; // 时间轮起始时间,每次调用TakeAllTimeout都会更新为当前时间戳(单位ms)
	long long llStartIdx; // 时间轮当前指向位置,需要向 iItemSize 取余才是真正的指针
};
stTimeout_t *AllocTimeout( int iSize ) // 初始化时间轮结构体
{
	stTimeout_t *lp = (stTimeout_t*)calloc( 1,sizeof(stTimeout_t) );	

	lp->iItemSize = iSize; //在 AllocEpoll中传入的是 60 * 1000 时间轮单位是ms
	lp->pItems = (stTimeoutItemLink_t*)calloc( 1,sizeof(stTimeoutItemLink_t) * lp->iItemSize );

	lp->ullStart = GetTickMS(); //获取当前时间戳,单位为ms
	lp->llStartIdx = 0;

	return lp;
}
void FreeTimeout( stTimeout_t *apTimeout )
{
	free( apTimeout->pItems );
	free ( apTimeout );
}
// 往时间轮添加定时器(事件),参数:apTimeout时间轮,apItem 插入的定时器,包含超时时间时间戳,allNow当前时间戳
// 该函数在co_eventloop 中调用,将active链表(该链表中的事件都是超时事件或可读写IO事件)
int AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,unsigned long long allNow )
{
	if( apTimeout->ullStart == 0 )
	{
		apTimeout->ullStart = allNow;
		apTimeout->llStartIdx = 0;
	}
	if( allNow < apTimeout->ullStart ) //异常情况,程序正常执行不会出现这种情况
	{
		co_log_err("CO_ERR: AddTimeout line %d allNow %llu apTimeout->ullStart %llu",
					__LINE__,allNow,apTimeout->ullStart);

		return __LINE__;
	}
	if( apItem->ullExpireTime < allNow ) // 异常情况
	{
		co_log_err("CO_ERR: AddTimeout line %d apItem->ullExpireTime %llu allNow %llu apTimeout->ullStart %llu",
					__LINE__,apItem->ullExpireTime,allNow,apTimeout->ullStart);

		return __LINE__;
	}
	unsigned long long diff = apItem->ullExpireTime - apTimeout->ullStart; // apTimeout->ullStart是上一次执行 TakeAllTimeout 的时间戳

	if( diff >= (unsigned long long)apTimeout->iItemSize ) 
	{
    //diff 不允许超过itemSize,如果diff远超iItemSize,则设为 iItemSize-1 ,然后计算插入槽中,这不会导致读取时间轮出错,因为从时间轮取出后还会根据事件的超时时间(co_eventloop中)再判断,如果没有超时还会加如时间轮,如果diff很大,那么多次取出并加入回时间轮后就会到达正确的位置
		diff = apTimeout->iItemSize - 1;
		co_log_err("CO_ERR: AddTimeout line %d diff %d",
					__LINE__,diff);

		//return __LINE__;
	}
	AddTail( apTimeout->pItems + ( apTimeout->llStartIdx + diff ) % apTimeout->iItemSize , apItem );

	return 0;
}
// 将指定槽中(当前时间与上次执行TakeAllTimeout时间的槽)的所有链表组成一个链表存放在apResult中,链表中的事件并不一定都是超时的,在eventLoop中还要再根据超时时间判断
inline void TakeAllTimeout( stTimeout_t *apTimeout,unsigned long long allNow,stTimeoutItemLink_t *apResult )
{
	if( apTimeout->ullStart == 0 ) //时间轮第一次调用该函数时初始化
	{
		apTimeout->ullStart = allNow;
		apTimeout->llStartIdx = 0;
	}

	if( allNow < apTimeout->ullStart ) //参处错误
	{
		return ;
	}
	int cnt = allNow - apTimeout->ullStart + 1; //计算时间差(每次调用TakeAllTimeout ullStart都会更新,更新赋值语句在下面)
	if( cnt > apTimeout->iItemSize ) //时间轮尺寸(槽数)有限制,超过了尺寸,将cnt赋值为槽数
	{ // 这里和 eventLoop 实现结合起来看, 时间差超过 iItemSize, 暂时赋值为 iTiemSize, 经过几轮最大超时时间后可以赶上正确的时间
		cnt = apTimeout->iItemSize;
	}
	if( cnt < 0 )//当前时间不可能比开始时间早,如果是就error
	{
		return;
	}
	for( int i = 0;i<cnt;i++) // 遍历时间轮上的槽(上一次遍历与当前时间之间的槽)
	{
		int idx = ( apTimeout->llStartIdx + i) % apTimeout->iItemSize;
    // 将apTimeout->pItems + idx 指向了槽上的链表,Join使链表连接到apReuslt中,apResult被更新为连接后的双向链表
    // apTimeout->pItems + idx 调用Join后链表的 tail 和 head 成员被置 null,相当与连接到apReuslt后pItems + idx链表被清除
    // apTimeout->pItems + idx 链表中的事件并不一定是超时的,在eventLoop 还会根据链表事件的超时时间判断
		Join<stTimeoutItem_t,stTimeoutItemLink_t>( apResult,apTimeout->pItems + idx  ); 
	}
	apTimeout->ullStart = allNow; //起始时间等于当前时间
	apTimeout->llStartIdx += cnt - 1;


}
static int CoRoutineFunc( stCoRoutine_t *co,void * )
{
	if( co->pfn )
	{
		co->pfn( co->arg );
	}
	co->cEnd = 1;

	stCoRoutineEnv_t *env = co->env;

	co_yield_env( env );

	return 0;
}



struct stCoRoutine_t *co_create_env( stCoRoutineEnv_t * env, const stCoRoutineAttr_t* attr,
		pfn_co_routine_t pfn,void *arg )
{

	stCoRoutineAttr_t at;
	if( attr )
	{
		memcpy( &at,attr,sizeof(at) );
	}
	if( at.stack_size <= 0 )
	{
		at.stack_size = 128 * 1024;
	}
	else if( at.stack_size > 1024 * 1024 * 8 )
	{
		at.stack_size = 1024 * 1024 * 8;
	}

	if( at.stack_size & 0xFFF ) 
	{
		at.stack_size &= ~0xFFF; //后12位必然被置0
		at.stack_size += 0x1000; //使得at.stack_size必然是4096(4k)的倍数
	}

	stCoRoutine_t *lp = (stCoRoutine_t*)malloc( sizeof(stCoRoutine_t) );
	
	memset( lp,0,(long)(sizeof(stCoRoutine_t))); 


	lp->env = env; // 初始化协程中的线程环境
	lp->pfn = pfn; // 初始化协程函数
	lp->arg = arg; // 初始化协程函数的参数

	stStackMem_t* stack_mem = NULL;
	if( at.share_stack )
	{
		stack_mem = co_get_stackmem( at.share_stack);
		at.stack_size = at.share_stack->stack_size;
	}
	else
	{
		stack_mem = co_alloc_stackmem(at.stack_size);
	}
	lp->stack_mem = stack_mem;

	lp->ctx.ss_sp = stack_mem->stack_buffer; //初始化协程栈
	lp->ctx.ss_size = at.stack_size; // 初始化协程栈大小,必然是4096的倍数

	lp->cStart = 0;
	lp->cEnd = 0;
	lp->cIsMain = 0;
	lp->cEnableSysHook = 0;
	lp->cIsShareStack = at.share_stack != NULL;

	lp->save_size = 0;
	lp->save_buffer = NULL;

	return lp;
}
//第一个参数是出参，传入的是初始化但置零的对象二级指针，第三个参数是协程入口运行函数，第四个参数是运行函数的参数
int co_create( stCoRoutine_t **ppco,const stCoRoutineAttr_t *attr,pfn_co_routine_t pfn,void *arg )
{
	if( !co_get_curr_thread_env() ) 
	{
		co_init_curr_thread_env(); // 全局变量 gCoEnvPerThread 在此被初始化
	}
  // co_create_env 返回一个协程(主协程),调用函数后:co->env = gCoEnvPerThread, co->pfn = pfn,co->arg = arg
  // co_get_curr_thread_env()返回线程全局唯一变量gCoEnvPerThread,co_create_env会对其分配内存并初始化
	stCoRoutine_t *co = co_create_env( co_get_curr_thread_env(), attr, pfn,arg );
	*ppco = co;
	return 0;
}
void co_free( stCoRoutine_t *co )
{
    if (!co->cIsShareStack) 
    {    
        free(co->stack_mem->stack_buffer);
        free(co->stack_mem);
    }   
    free( co );
}
void co_release( stCoRoutine_t *co )
{
    co_free( co );
}

void co_swap(stCoRoutine_t* curr, stCoRoutine_t* pending_co);

void co_resume( stCoRoutine_t *co )
{
	stCoRoutineEnv_t *env = co->env;
	stCoRoutine_t *lpCurrRoutine = env->pCallStack[ env->iCallStackSize - 1 ];
	if( !co->cStart )
	{
		coctx_make( &co->ctx,(coctx_pfn_t)CoRoutineFunc,co,0 );
		co->cStart = 1;
	}
	env->pCallStack[ env->iCallStackSize++ ] = co;
	co_swap( lpCurrRoutine, co );


}
void co_yield_env( stCoRoutineEnv_t *env )
{
	
	stCoRoutine_t *last = env->pCallStack[ env->iCallStackSize - 2 ];
	stCoRoutine_t *curr = env->pCallStack[ env->iCallStackSize - 1 ];

	env->iCallStackSize--;

	co_swap( curr, last);
}

void co_yield_ct()
{

	co_yield_env( co_get_curr_thread_env() );
}
void co_yield( stCoRoutine_t *co )
{
	co_yield_env( co->env );
}

void save_stack_buffer(stCoRoutine_t* occupy_co)
{
	///copy out
	stStackMem_t* stack_mem = occupy_co->stack_mem;
	int len = stack_mem->stack_bp - occupy_co->stack_sp;

	if (occupy_co->save_buffer)
	{
		free(occupy_co->save_buffer), occupy_co->save_buffer = NULL;
	}

	occupy_co->save_buffer = (char*)malloc(len); //malloc buf;
	occupy_co->save_size = len;

	memcpy(occupy_co->save_buffer, occupy_co->stack_sp, len);
}

void co_swap(stCoRoutine_t* curr, stCoRoutine_t* pending_co)
{
 	stCoRoutineEnv_t* env = co_get_curr_thread_env();

	//get curr stack sp
	char c;
	curr->stack_sp= &c;

	if (!pending_co->cIsShareStack)
	{
		env->pending_co = NULL;
		env->occupy_co = NULL;
	}
	else 
	{
		env->pending_co = pending_co;
		//get last occupy co on the same stack mem
		stCoRoutine_t* occupy_co = pending_co->stack_mem->occupy_co;
		//set pending co to occupy thest stack mem;
		pending_co->stack_mem->occupy_co = pending_co;

		env->occupy_co = occupy_co;
		if (occupy_co && occupy_co != pending_co)
		{
			save_stack_buffer(occupy_co);
		}
	}

	//swap context
	coctx_swap(&(curr->ctx),&(pending_co->ctx) );

	//stack buffer may be overwrite, so get again;
	stCoRoutineEnv_t* curr_env = co_get_curr_thread_env();
	stCoRoutine_t* update_occupy_co =  curr_env->occupy_co;
	stCoRoutine_t* update_pending_co = curr_env->pending_co;
	
	if (update_occupy_co && update_pending_co && update_occupy_co != update_pending_co)
	{
		//resume stack buffer
		if (update_pending_co->save_buffer && update_pending_co->save_size > 0)
		{
			memcpy(update_pending_co->stack_sp, update_pending_co->save_buffer, update_pending_co->save_size);
		}
	}
}



//int poll(struct pollfd fds[], nfds_t nfds, int timeout);
// { fd,events,revents }
struct stPollItem_t ;
struct stPoll_t : public stTimeoutItem_t 
{
	struct pollfd *fds;
	nfds_t nfds; // typedef unsigned long int nfds_t;

	stPollItem_t *pPollItems;

	int iAllEventDetach;

	int iEpollFd;

	int iRaiseCnt;


};
struct stPollItem_t : public stTimeoutItem_t
{
	struct pollfd *pSelf;
	stPoll_t *pPoll;

	struct epoll_event stEvent;
};
/*
 *   EPOLLPRI 		POLLPRI    // There is urgent data to read.  
 *   EPOLLMSG 		POLLMSG
 *
 *   				POLLREMOVE
 *   				POLLRDHUP
 *   				POLLNVAL
 *
 * */
static uint32_t PollEvent2Epoll( short events )
{
	uint32_t e = 0;	
	if( events & POLLIN ) 	e |= EPOLLIN;
	if( events & POLLOUT )  e |= EPOLLOUT;
	if( events & POLLHUP ) 	e |= EPOLLHUP;
	if( events & POLLERR )	e |= EPOLLERR;
	if( events & POLLRDNORM ) e |= EPOLLRDNORM;
	if( events & POLLWRNORM ) e |= EPOLLWRNORM;
	return e;
}
static short EpollEvent2Poll( uint32_t events )
{
	short e = 0;	
	if( events & EPOLLIN ) 	e |= POLLIN;
	if( events & EPOLLOUT ) e |= POLLOUT;
	if( events & EPOLLHUP ) e |= POLLHUP;
	if( events & EPOLLERR ) e |= POLLERR;
	if( events & EPOLLRDNORM ) e |= POLLRDNORM;
	if( events & EPOLLWRNORM ) e |= POLLWRNORM;
	return e;
}

static __thread stCoRoutineEnv_t* gCoEnvPerThread = NULL; //__thread变量每一个线程有一份独立实体，各个线程的值互不干扰,即该变量不是多线程共享的

void co_init_curr_thread_env()
{
	gCoEnvPerThread = (stCoRoutineEnv_t*)calloc( 1, sizeof(stCoRoutineEnv_t) ); //分配内存
	stCoRoutineEnv_t *env = gCoEnvPerThread;

	env->iCallStackSize = 0;
	struct stCoRoutine_t *self = co_create_env( env, NULL, NULL,NULL );
	self->cIsMain = 1;

	env->pending_co = NULL;
	env->occupy_co = NULL;

	coctx_init( &self->ctx );

	env->pCallStack[ env->iCallStackSize++ ] = self; // iCallStackSize的初始值为1

	stCoEpoll_t *ev = AllocEpoll();
	SetEpoll( env,ev );
}
stCoRoutineEnv_t *co_get_curr_thread_env()
{
	return gCoEnvPerThread;
}

void OnPollProcessEvent( stTimeoutItem_t * ap )
{
	stCoRoutine_t *co = (stCoRoutine_t*)ap->pArg;
	co_resume( co );
}

void OnPollPreparePfn( stTimeoutItem_t * ap,struct epoll_event &e,stTimeoutItemLink_t *active )
{
	stPollItem_t *lp = (stPollItem_t *)ap; // stPollItem_t是子类, public继承了 stTimeoutItem_t
	lp->pSelf->revents = EpollEvent2Poll( e.events ); // 将epoll事件转换为poll事件复制给pSelf(struct poll_fd类型)


	stPoll_t *pPoll = lp->pPoll;
	pPoll->iRaiseCnt++;

	if( !pPoll->iAllEventDetach )
	{
		pPoll->iAllEventDetach = 1;

		RemoveFromLink<stTimeoutItem_t,stTimeoutItemLink_t>( pPoll ); // 将pPoll元素从pPoll->pLink链表中移除

		AddTail( active,pPoll ); // 将 pPoll添加到 active 链表

	}
}

// 第一个参数 ctx 通常由 co_get_epoll_ct() 获取,是线程全局唯一的变量
void co_eventloop( stCoEpoll_t *ctx,pfn_co_eventloop_t pfn,void *arg )
{
	if( !ctx->result )
	{ //ctx->result是存储了epoll事件的结果,里面封装了 epoll_event 数组
    //数组大小为_EPOLL_SIZE = 1024*10
		ctx->result =  co_epoll_res_alloc( stCoEpoll_t::_EPOLL_SIZE );
	}
	co_epoll_res *result = ctx->result;


	for(;;)
	{
    //ctx->iEpollFd 是使用epoll_create初始化的,一个线程只有一个 ctx 全局变量,因此一个线程只有一个ctx->iEpollFd,_EPOLL_SIZE是epoll_create的参数
		int ret = co_epoll_wait( ctx->iEpollFd,result,stCoEpoll_t::_EPOLL_SIZE, 1 );//调用epoll_wait,相当于调用epoll_wait( ctx->iEpollFd,result->events,stCoEpoll_t::_EPOLL_SIZE, 1 )

		stTimeoutItemLink_t *active = (ctx->pstActiveList);
		stTimeoutItemLink_t *timeout = (ctx->pstTimeoutList);

		memset( timeout,0,sizeof(stTimeoutItemLink_t) );

		for(int i=0;i<ret;i++) // 遍历所有epoll触发的事件
		{
			stTimeoutItem_t *item = (stTimeoutItem_t*)result->events[i].data.ptr; //result->event 类型是struct epoll_event * 这个是epoll事件结构体
      //epoll_event.data.ptr 的类型是 void*,是交给用户自由定义使用的,可用来存储任意类型变量
			if( item->pfnPrepare ) //说明这个 item 是 co_poll_inner 中注册的 item
			{
        //这个部分处理的逻辑是,调用co_poll时,如果设置了timeout 参数,则会添加一个超时事件,并从co_poll 中切换回co_event_loop协程,如果在timeout未到达时网络事件到来,则需要切换回 co_poll 去处理网络事件,那么时间轮中的事件就应该移除,否则之后timeout时间一到会重复执行同一个 co_poll 导致出错
        // pfnPrepare 基本是被赋值为 OnPollPreparePfn,赋值的操作在 hook 后的polll>co_poll->co_poll_inner函数中
        // 将co_poll_inner 中添加的 timeout 事件删除，并将 active 添加到active list中
				item->pfnPrepare( item,result->events[i],active );
			}
			else
			{
				AddTail( active,item ); //将item添加到active链表中(尾部)
			}
		}


		unsigned long long now = GetTickMS(); // 获取当前时间戳,单位ms
		TakeAllTimeout( ctx->pTimeout,now,timeout ); // timeout 是出参,TakeAllTimeout 使当前时间now与上次执行TakeAllTimout的时间戳之间的所有槽的所有链表都连接到 timeout 尾部.注意 timeout 链表中的并不一定是超时事件,也可能是恰好槽数符合但并不超时的事件,下面会根据ullExpireTime成员判断

		stTimeoutItem_t *lp = timeout->head;
		while( lp ) // 遍历
		{
			//printf("raise timeout %p\n",lp);
			lp->bTimeout = true; //bTimeout为 true 不表示超时了,而是表示这个事件是时间轮里的事件, 而不是epoll网络事件
			lp = lp->pNext;
		}
    // 将 active 链表和 timeout 链表连接,赋值到 active 链表返回, timeout 链表清空
		Join<stTimeoutItem_t,stTimeoutItemLink_t>( active,timeout );
    // 此时的 active 链表,其内部的所有事件(超时事件以及已经可读写的IO事件)都是当前需要进行处理的
		lp = active->head;
		while( lp )
		{

			PopHead<stTimeoutItem_t,stTimeoutItemLink_t>( active ); //移除链表头部,这里 lp 已经拿到了头部的指针了,所以移除链表后也可以释放
            if (lp->bTimeout && now < lp->ullExpireTime) // 注意与的关系,lb->bTimeout表示这是时间轮里的事件而非网络事件,now < lp->ullExpireTime,则表示这个事件没有超时,因为TakeAllTimeout取出的并不都是超时事件
			{
				int ret = AddTimeout(ctx->pTimeout, lp, now);//没有超时的事件将它添加回时间轮中
				if (!ret) 
				{
					lp->bTimeout = false;
					lp = active->head;// 未超时的事件加入时间轮后,lp指向active链表下一个事件,这里使用active->head是因为之前lp之间的事件已经被PopHead了
					continue;
				}
			}
			if( lp->pfnProcess ) //对于已经到达时间的事件或者网络事件,执行 pfnProcess
			{
				lp->pfnProcess( lp );
			}

			lp = active->head;
		}
		if( pfn )
		{
			if( -1 == pfn( arg ) )
			{
				break;
			}
		}

	} // end for
}
void OnCoroutineEvent( stTimeoutItem_t * ap )
{
	stCoRoutine_t *co = (stCoRoutine_t*)ap->pArg;
	co_resume( co );
}

// stCoEpoll_t 内的指针变量分配内存, iEpollFd 成员使用epoll_create初始化
stCoEpoll_t *AllocEpoll()
{
	stCoEpoll_t *ctx = (stCoEpoll_t*)calloc( 1,sizeof(stCoEpoll_t) );

	ctx->iEpollFd = co_epoll_create( stCoEpoll_t::_EPOLL_SIZE );// 调用epoll_create
	ctx->pTimeout = AllocTimeout( 60 * 1000 );
	
	ctx->pstActiveList = (stTimeoutItemLink_t*)calloc( 1,sizeof(stTimeoutItemLink_t) );
	ctx->pstTimeoutList = (stTimeoutItemLink_t*)calloc( 1,sizeof(stTimeoutItemLink_t) );


	return ctx;
}

void FreeEpoll( stCoEpoll_t *ctx )
{
	if( ctx )
	{
		free( ctx->pstActiveList );
		free( ctx->pstTimeoutList );
		FreeTimeout( ctx->pTimeout );
		co_epoll_res_free( ctx->result );
	}
	free( ctx );
}

stCoRoutine_t *GetCurrCo( stCoRoutineEnv_t *env )
{
	return env->pCallStack[ env->iCallStackSize - 1 ];
}
stCoRoutine_t *GetCurrThreadCo( )
{
	stCoRoutineEnv_t *env = co_get_curr_thread_env(); //env = gCoEnvPerThread
	if( !env ) return 0;
	return GetCurrCo(env); //env->pCallStack[ env->iCallStackSize - 1 ]
}



typedef int (*poll_pfn_t)(struct pollfd fds[], nfds_t nfds, int timeout);
int co_poll_inner( stCoEpoll_t *ctx,struct pollfd fds[], nfds_t nfds, int timeout, poll_pfn_t pollfunc)
{
    if (timeout == 0)
	{
		return pollfunc(fds, nfds, timeout);
	}
	if (timeout < 0)
	{
		timeout = INT_MAX;
	}
	int epfd = ctx->iEpollFd;
	stCoRoutine_t* self = co_self(); //当前协程变量,self = gCoEnvPerThread->pCallStack[ gCoEnvPerThread->iCallStackSize - 1 ]

	//1.struct change, 将epoll结构转换为poll结构
	stPoll_t& arg = *((stPoll_t*)malloc(sizeof(stPoll_t)));
	memset( &arg,0,sizeof(arg) );

	arg.iEpollFd = epfd;
	arg.fds = (pollfd*)calloc(nfds, sizeof(pollfd));
	arg.nfds = nfds;

	stPollItem_t arr[2];
	if( nfds < sizeof(arr) / sizeof(arr[0]) && !self->cIsShareStack)
	{
		arg.pPollItems = arr;
	}	
	else
	{
		arg.pPollItems = (stPollItem_t*)malloc( nfds * sizeof( stPollItem_t ) );
	}
	memset( arg.pPollItems,0,nfds * sizeof(stPollItem_t) );

	arg.pfnProcess = OnPollProcessEvent; // 事件发生时的回调函数, 其主要功能是恢复pArg指向的协程
	arg.pArg = GetCurrCo( co_get_curr_thread_env() ); // arg.pArg = gCoEnvPerThread->pCallStack[ gCoEnvPerThread->iCallStackSize - 1 ]
	
	
	//2. add epoll
	for(nfds_t i=0;i<nfds;i++)
	{
		arg.pPollItems[i].pSelf = arg.fds + i;
		arg.pPollItems[i].pPoll = &arg;

		arg.pPollItems[i].pfnPrepare = OnPollPreparePfn;
		struct epoll_event &ev = arg.pPollItems[i].stEvent;

		if( fds[i].fd > -1 )
		{
			ev.data.ptr = arg.pPollItems + i;
			ev.events = PollEvent2Epoll( fds[i].events ); //将poll的属性转为epoll的属性

			int ret = co_epoll_ctl( epfd,EPOLL_CTL_ADD, fds[i].fd, &ev ); // epoll_wait 的调用在 co_eventloop 中
			if (ret < 0 && errno == EPERM && nfds == 1 && pollfunc != NULL)
			{
				if( arg.pPollItems != arr )
				{
					free( arg.pPollItems );
					arg.pPollItems = NULL;
				}
				free(arg.fds);
				free(&arg);
				return pollfunc(fds, nfds, timeout);
			}
		}
		//if fail,the timeout would work
	}

	//3.add timeout
  //对应 poll中的timeout,系统的poll是会阻塞的,这里改造通过时间轮和协程切换变为非阻塞
	unsigned long long now = GetTickMS(); //获取当前时间的时间戳,单位ms
	arg.ullExpireTime = now + timeout;
	int ret = AddTimeout( ctx->pTimeout,&arg,now );//添加到时间轮中
	int iRaiseCnt = 0;
	if( ret != 0 )
	{
		co_log_err("CO_ERR: AddTimeout ret %d now %lld timeout %d arg.ullExpireTime %lld",
				ret,now,timeout,arg.ullExpireTime);
		errno = EINVAL;
		iRaiseCnt = -1;

	}
    else
	{
		co_yield_env( co_get_curr_thread_env() ); //添加完超时事件后切换协程,通常是切换回co_eventloop
		iRaiseCnt = arg.iRaiseCnt;
	}

    {
		//clear epoll status and memory
		RemoveFromLink<stTimeoutItem_t,stTimeoutItemLink_t>( &arg );
		for(nfds_t i = 0;i < nfds;i++)
		{
			int fd = fds[i].fd;
			if( fd > -1 )
			{
				co_epoll_ctl( epfd,EPOLL_CTL_DEL,fd,&arg.pPollItems[i].stEvent );
			}
			fds[i].revents = arg.fds[i].revents;
		}


		if( arg.pPollItems != arr )
		{
			free( arg.pPollItems );
			arg.pPollItems = NULL;
		}

		free(arg.fds);
		free(&arg);
	}

	return iRaiseCnt;
}
// 参数timeout 对应 poll的timeout,本来是会阻塞等待的,在co_poll_inner中被改为加入到时间轮中,然后切换协程
// co_poll是poll hook 后最终会调用的函数,这个函数非常重要,它最终调用了 epoll_ctl 添加了监听事件,并且会调用co_yeild_env 切换协程
// co_poll的使用策略:在调用read,write,accept之前,先调用co_poll,改用 epoll去监听事件,并切换到epoll_wait去监听,如果有网络事件到来,就可以co_resume切换回co_poll然后再接着执行read,write,accept,如果没有网络事件到来,等待timeout 时间后再切换回来,之后read,write如果还是失败再做处理,到有了timeout时间可以大大增加成功率,且因为有协程切换不会阻塞
int	co_poll( stCoEpoll_t *ctx,struct pollfd fds[], nfds_t nfds, int timeout_ms )
{
	return co_poll_inner(ctx, fds, nfds, timeout_ms, NULL);
}

void SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev )
{
	env->pEpoll = ev;
}
stCoEpoll_t *co_get_epoll_ct()
{
	if( !co_get_curr_thread_env() )
	{
		co_init_curr_thread_env();
	}
	return co_get_curr_thread_env()->pEpoll;
}
struct stHookPThreadSpec_t
{
	stCoRoutine_t *co;
	void *value;

	enum 
	{
		size = 1024
	};
};
void *co_getspecific(pthread_key_t key)
{
	stCoRoutine_t *co = GetCurrThreadCo();
	if( !co || co->cIsMain )
	{
		return pthread_getspecific( key );
	}
	return co->aSpec[ key ].value;
}
int co_setspecific(pthread_key_t key, const void *value)
{
	stCoRoutine_t *co = GetCurrThreadCo();
	if( !co || co->cIsMain )
	{
		return pthread_setspecific( key,value );
	}
	co->aSpec[ key ].value = (void*)value;
	return 0;
}



void co_disable_hook_sys()
{
	stCoRoutine_t *co = GetCurrThreadCo();
	if( co )
	{
		co->cEnableSysHook = 0;
	}
}
bool co_is_enable_sys_hook()
{
	stCoRoutine_t *co = GetCurrThreadCo();
	return ( co && co->cEnableSysHook );
}

stCoRoutine_t *co_self()
{
	return GetCurrThreadCo(); //return gCoEnvPerThread->pCallStack[ gCoEnvPerThread->iCallStackSize - 1 ]
}

//co cond
struct stCoCond_t;
struct stCoCondItem_t 
{
	stCoCondItem_t *pPrev;
	stCoCondItem_t *pNext;
	stCoCond_t *pLink;

	stTimeoutItem_t timeout;
};
struct stCoCond_t
{
	stCoCondItem_t *head;
	stCoCondItem_t *tail;
};
static void OnSignalProcessEvent( stTimeoutItem_t * ap )
{
	stCoRoutine_t *co = (stCoRoutine_t*)ap->pArg;
	co_resume( co );
}

stCoCondItem_t *co_cond_pop( stCoCond_t *link );
int co_cond_signal( stCoCond_t *si )
{
	stCoCondItem_t * sp = co_cond_pop( si );
	if( !sp ) 
	{
		return 0;
	}
	RemoveFromLink<stTimeoutItem_t,stTimeoutItemLink_t>( &sp->timeout );

	AddTail( co_get_curr_thread_env()->pEpoll->pstActiveList,&sp->timeout );

	return 0;
}
int co_cond_broadcast( stCoCond_t *si )
{
	for(;;)
	{
		stCoCondItem_t * sp = co_cond_pop( si );
		if( !sp ) return 0;

		RemoveFromLink<stTimeoutItem_t,stTimeoutItemLink_t>( &sp->timeout );

		AddTail( co_get_curr_thread_env()->pEpoll->pstActiveList,&sp->timeout );
	}

	return 0;
}


int co_cond_timedwait( stCoCond_t *link,int ms )
{
	stCoCondItem_t* psi = (stCoCondItem_t*)calloc(1, sizeof(stCoCondItem_t));
	psi->timeout.pArg = GetCurrThreadCo();
	psi->timeout.pfnProcess = OnSignalProcessEvent;

	if( ms > 0 )
	{
		unsigned long long now = GetTickMS();
		psi->timeout.ullExpireTime = now + ms;

		int ret = AddTimeout( co_get_curr_thread_env()->pEpoll->pTimeout,&psi->timeout,now );
		if( ret != 0 )
		{
			free(psi);
			return ret;
		}
	}
	AddTail( link, psi);

	co_yield_ct();


	RemoveFromLink<stCoCondItem_t,stCoCond_t>( psi );
	free(psi);

	return 0;
}
stCoCond_t *co_cond_alloc()
{
	return (stCoCond_t*)calloc( 1,sizeof(stCoCond_t) );
}
int co_cond_free( stCoCond_t * cc )
{
	free( cc );
	return 0;
}


stCoCondItem_t *co_cond_pop( stCoCond_t *link )
{
	stCoCondItem_t *p = link->head;
	if( p )
	{
		PopHead<stCoCondItem_t,stCoCond_t>( link );
	}
	return p;
}
