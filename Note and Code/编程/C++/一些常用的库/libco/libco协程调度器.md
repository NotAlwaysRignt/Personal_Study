`libco`实现了网络函数的hook,实现了协程的切换,并通过`epoll`实现了协程调度器,那么它是如何将它们结合起来的呢
这里通过`libco`提供的示例文件`example_echosvr.cpp`来讲解
该函数实现了一个`echo`服务器,并支持多进程,每个进程可以支持多个协程
在讲解完这个例子后,我们就可以大致认识一个协程调度的过程,接着再总结一下协程调度的思路
### `example_echosvr.cpp`讲解
`example_echosvr.cpp`中的所有协程在`main`函数中初始化时先调用`co_create`创建好,主要的两个自定义协程为`readwrite_routine`和`accept_routine`

`accept_routine`负责调用`accept`接收新的客户端连接,得到一个新的文件描述符fd
`readwrite_routine`实现了`read`数据并调用`write`回写的逻辑

`accept_routine`和`readwrite_routine`联系的关键在于全局变量`static stack<task_t*> g_readwrite;`
其中
```cpp
struct task_t
{
	stCoRoutine_t *co;
	int fd;
};
```
`g_readwrite`初始化时存放创建好的协程,当`accept_routine`中调用`accept`获得新的文件描述符后,会从`g_readwrite`取出一个`task_t`对象来,里面封装了一个可用的`readwrite_routine`协程,然后调用`co_resume`运行它.

`accept_routine`和`readwrite_routine`都是在main函数一开始用`co_create`创建的,并且里面都使用了`for(;;)`进入死循环,确保这个协程一直存在不退出
其中`accept_routine`中不断地调用`co_poll( co_get_epoll_ct(),&pf,1,1000 )`使得能够不断地在`accept_routine`和`co_eventloop`中切换
因为即使没有网络事件到来,也可以在 timeout 时间以后(这里设为1000)触发超时事件切换回`accept_routine`
调用`accept`后,如果没有网络事件,则继续进入下一个 for 循环调用`co_poll`

下面看看main函数中初始化`accept_routine`和`readwrite_routine`的代码
```cpp
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
```
#### readwrite_routine
```cpp
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
				continue;
			}
			close( fd );
			break;
		}

	}
	return 0;
}

```
#### accept_routine
```cpp
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
```

### 协程调度器总结
以通过 libco 源码的学习,可以简单地对协程调度过程总结如下:
* 首先注册`epoll`,然后调用`epoll_wait`监听连接
* `epoll`发现新的连接事件后,使用`accpet`获取新的文件描述符
* hook系统的的`read`,`write`,将其设置为非阻塞的,而且被hook后的`read`,`write`不是直接调用系统的`read`,`write`,而是在调用read,write 前先用`epoll_ctl`添加读写监听事件,然后切换协程到`epoll_wait`交给`epoll`去判断是否可读写,如果可读写,则把协程切换回去,调用`read`,`write`,当然为了一直没有网络事件到来而无法切换回来的情况,还需要添加一个超时事件监听,超时时间一到也有切换回来执行非阻塞的`read`,`write`

完成以上步骤,那么就可以最大限度地减少阻塞了,使用`epoll`监听一切网络事件

#### 从`co_sys_hook_call.cpp`看一种通用的情况
libco使用了hook技巧使得所有网络事件都可以被改造为libco的协程调度模式,因此理解了 hook 后的行为可以帮助我们理解协程调度器的运行过程

以`co_sys_hook_call.cpp`为例,对于被 hook 的`read`和`write`,一方面会将它们设置为非阻塞模式,具体的做法是hook系统的socket,在调用socket时调用`fcntl`设置为非阻塞socket,`fcntl`也被libco hook 了
另一方面,被hook后`read`,`write`不是直接调用系统的`read`,`write`,而是先调用`poll`(本质是调用了`co_poll`),执行的操作就是调用`epoll_ctl`,然后切换协程到`co_eventloop`,`co_eventloop`主要通过`epoll_wati`监听网络事件,以及检查时间轮中存放的超时事件,把`epoll`返回的网络事件和时间轮中的超时事件构成一个链表,这个链表就是一次loop中要去处理的事件,使用`co_resume`切换到这些协程中去处理它们
为了避免连接一直没有事件到来(比如客户端断开了)的情况,`co_poll`还设置了个超时时间(`libco`使用了时间轮来检查超时事件),也就是在注册网络事件监听时一般还会注册一个超时事件监听,超时时间一到也会切换回`co_poll`然后执行系统的`read`,`write`
