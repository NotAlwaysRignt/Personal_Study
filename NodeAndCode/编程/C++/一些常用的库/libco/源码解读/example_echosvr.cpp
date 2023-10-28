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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <stack>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#ifdef __FreeBSD__
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#endif

using namespace std;
struct task_t
{
	stCoRoutine_t *co;
	int fd;
};

static stack<task_t*> g_readwrite;
static int g_listen_fd = -1;
static int SetNonBlock(int iSock)
{
    int iFlags;

    iFlags = fcntl(iSock, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(iSock, F_SETFL, iFlags);
    return ret;
}

static void *readwrite_routine( void *arg )
{

	co_enable_hook_sys();

	task_t *co = (task_t*)arg;
	char buf[ 1024 * 16 ];
	for(;;)
	{
		if( -1 == co->fd )
		{
			g_readwrite.push( co ); //static stack<task_t*> g_readwrite;
			co_yield_ct();
			continue;
		}

		int fd = co->fd;
		co->fd = -1; //这里应该设为-1,在下面for循环break 后才会继续调用g_readwrite.push( co );否则会出错

		for(;;) //处理一个客户端连接,在客户端连接断开前会在此for循环中,这意味着一个客户端连接会占用一个 readwrite_routine协程, 并发请求时如果协程数不够则无法处理
		{
			struct pollfd pf = { 0 };
			pf.fd = fd;
			pf.events = (POLLIN|POLLERR|POLLHUP);
			co_poll( co_get_epoll_ct(),&pf,1,1000); //延时,但不会阻塞,而是协程被切换(co_poll进行了改造,加入到了时间轮去了),之后会回来,但这里不是普通的延时,而是使pf.fd = fd 并监听了读事件,以便在可读事件到来时及时切换回来
      //co_poll 延时结束后往下执行,如果此时还是没有可读,则ret返回负数,接着又回到for,返回for后调用poll这样就可以在没有可读但连接又没有断开时先切换到别的协程了
			int ret = read( fd,buf,sizeof(buf) ); //这里实现的是echo的逻辑，读出来，直接回写
			if( ret > 0 )
			{
				ret = write( fd,buf,ret );
			}
			if( ret > 0 || ( -1 == ret && EAGAIN == errno ) )
			{
				continue; // 当客户端没有关闭TCP连接时,continue,保持连接,而不是调用下面的close
			}
			close( fd );
			break;
		}

	}
	return 0;
}
int co_accept(int fd, struct sockaddr *addr, socklen_t *len );
static void *accept_routine( void * )
{
	co_enable_hook_sys();
	printf("accept_routine\n");
	fflush(stdout);
	for(;;)
	{
		//printf("pid %ld g_readwrite.size %ld\n",getpid(),g_readwrite.size());
		if( g_readwrite.empty() )
		{
			printf("empty\n"); //sleep,表示当前没有可用的readwrite_routine协程,新客户连接将无法处理
			struct pollfd pf = { 0 };
			pf.fd = -1;
			poll( &pf,1,1000); //这里调用的是被hook了的poll,这里用于sleep 1000 ms

			continue; //回到 for

		}
		struct sockaddr_in addr; //maybe sockaddr_un;
		memset( &addr,0,sizeof(addr) );
		socklen_t len = sizeof(addr);

		int fd = co_accept(g_listen_fd, (struct sockaddr *)&addr, &len); //co_accept 是无阻塞的
		if( fd < 0 ) //如果没有新的客户端连接到来,则会进入这里
		{
			struct pollfd pf = { 0 };
			pf.fd = g_listen_fd; //g_listen_fd 是调用 CreateSocket 函数得到的
			pf.events = (POLLIN|POLLERR|POLLHUP);
			co_poll( co_get_epoll_ct(),&pf,1,1000 ); //调用co_poll时,协程会切出,下次恢复时往下执行
      //上面co_poll最后其实会调用的还是epoll(系统支持的情况下),epoll结构体由 co_get_epoll_ct() 返回,而main函数中co_eventloop 监听的也是 co_get_epoll_ct() 的 epoll
			continue;
		}
		if( g_readwrite.empty() )
		{
			close( fd );
			continue;
		}
		SetNonBlock( fd );
		task_t *co = g_readwrite.top();
		co->fd = fd;
		g_readwrite.pop();
		co_resume( co->co ); //main 函数中 co_create( &(task->co),NULL,readwrite_routine,task ); 因此这里是恢复readwrite_routine的执行
	} // end for
	return 0;
}

static void SetAddr(const char *pszIP,const unsigned short shPort,struct sockaddr_in &addr)
{
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(shPort);
	int nIP = 0;
	if( !pszIP || '\0' == *pszIP   
	    || 0 == strcmp(pszIP,"0") || 0 == strcmp(pszIP,"0.0.0.0") 
		|| 0 == strcmp(pszIP,"*") 
	  )
	{
		nIP = htonl(INADDR_ANY);
	}
	else
	{
		nIP = inet_addr(pszIP);
	}
	addr.sin_addr.s_addr = nIP;

}

static int CreateTcpSocket(const unsigned short shPort /* = 0 */,const char *pszIP /* = "*" */,bool bReuse /* = false */)
{
	int fd = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);//这个socket函数是hook后的socket
	if( fd >= 0 )
	{
		if(shPort != 0)
		{
			if(bReuse)
			{
				int nReuseAddr = 1;
				setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&nReuseAddr,sizeof(nReuseAddr));
			}
			struct sockaddr_in addr ;
			SetAddr(pszIP,shPort,addr);
			int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
			if( ret != 0)
			{
				close(fd);
				return -1;
			}
		}
	}
	return fd;
}


int main(int argc,char *argv[])
{
	if(argc<5){
		printf("Usage:\n"
               "example_echosvr [IP] [PORT] [TASK_COUNT] [PROCESS_COUNT]\n"
               "example_echosvr [IP] [PORT] [TASK_COUNT] [PROCESS_COUNT] -d   # daemonize mode\n");
		return -1;
	}
	const char *ip = argv[1];
	int port = atoi( argv[2] );
	int cnt = atoi( argv[3] );
	int proccnt = atoi( argv[4] );
	bool deamonize = argc >= 6 && strcmp(argv[5], "-d") == 0;
  
	g_listen_fd = CreateTcpSocket( port,ip,true ); //创建socket,并用bind绑定ip端口,注意,socket相关的API都被hook了
	listen( g_listen_fd,1024 );
	if(g_listen_fd==-1){
		printf("Port %d is in use\n", port);
		return -1;
	}
	printf("listen %d %s:%d\n",g_listen_fd,ip,port);

	SetNonBlock( g_listen_fd );

	for(int k=0;k<proccnt;k++)
	{

		pid_t pid = fork();//用fork的方式实现多进程,子进程中返回值为0,父进程中返回的是子进程的pid(>0),如果失败,返回-1
		if( pid > 0 ) //父进程会执行到此，因为创建了5次子进程，五次都会continue
		{
			continue;
		}
		else if( pid < 0 )
		{
			break;
		}
		for(int i=0;i<cnt;i++)
		{
			task_t * task = (task_t*)calloc( 1,sizeof(task_t) );//calloc 分配的是数组,第一个参数是数组的长度,第二个长度是每个元素的内存大小,并且会自动全部初始化为0
			task->fd = -1;

			co_create( &(task->co),NULL,readwrite_routine,task );
			co_resume( task->co );

		}
		stCoRoutine_t *accept_co = NULL;
		co_create( &accept_co,NULL,accept_routine,0 ); //accpet_co会被赋值co_create_env的返回值
		co_resume( accept_co ); //运行至此时 accept_routine 会被执行一次,然后在调用到 co_poll 时被切出继续执行下面的 co_eventloop

		co_eventloop( co_get_epoll_ct(),0,0 );

		exit(0);
	}
	if(!deamonize) wait(NULL); //调用wait时，父进程会阻塞在此，直到所有子进程结束
	return 0;
}

