### channel poller, eventloop的协作
对应代码位置: https://github.com/chenshuo/recipes/tree/master/reactor/s01
这个目录下实现了用 Channel 关注 timefd 可读的 demo
`test3.cpp`内容如下:
```cpp
#include "Channel.h"                                                                                                      
#include "EventLoop.h"
         
#include <stdio.h>
#include <sys/timerfd.h>
         
muduo::EventLoop* g_loop;
         
//程序功能:程序启动5s后调用 timeout()函数然后结束
         
void timeout()
{        
  printf("Timeout!\n");
  g_loop->quit(); //这里会使EventLoop::loop 终止
}        
         
int main()
{        
  muduo::EventLoop loop;
  g_loop = &loop;
         
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  muduo::Channel channel(&loop, timerfd); //文件描述符先注册到 channel 里
  channel.setReadCallback(timeout);
  channel.enableReading(); //这一步会注册读事件(events = POLLIN | POLLPRI),然后再调用 Loop::updateChannel 把这个Channel注>册到 Poller里头去
  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd, 0, &howlong, NULL); //给这个timerfd 注册事件
  
  loop.loop(); //while循环,不断调用poll()方法,检查是否有 active 事件,如果有,就调用其相应回调函数
  
  ::close(timerfd);                                                                                                       
} 
```
### poll函数简介
首先了解 poll 的用法,`#include <poll.h>` poll.h 中的重要结构体成员
```cpp
struct pollfd{
	int fd;			//文件描述符
	short events;	//等待的事件
	short revents;	//实际发生的事件
};
```
然后调用`int poll(struct pollfd *fds, nfds_t nfds, int timeout);`第一个参数是`struct pollfd`指针,使用时应该传入一个数组对象,即我们要关注的所有IO事件,函数返回活跃事件的个数,之后我们要遍历传入数组的每个 strcut pollfd,如果其 revents !=0 ,则说明有事件到来(其值不一定是等待的事件,也可能是其它事件如出错了),根据 revents 我们即可做相应的处理

### Channel对象简介
Channel就是对一个 fd 的封装,一个Channel 对应一个 IO 事件,一个 Channel 封装了一个 IO 事件的 fd 以及这个
例子中当实例化一个 Channel 后,就可以根据这个 Channel 来构建一个 struct pollfd 了.
例子中用`timerfd_create`注册完 fd 后,把`fd` 和要监听的事件传入 Channel 中保存, 一个进程中所有Channel都共用一个 EventLoop 对象, EventLoop 对象作为 Channel 的成员函数
Channel只负责保存 fd,回调函数,要监听的事件,但监听这个行为,即调用 `poll` 函数这个行为交给了 Poller
### Poller
#### Poller 与 Channel 的关系
由上面知道了 Channel 是对实践 fd 的封装, 这个 fd 最终是要给 poll 监听的, Poller 就是对 poll的监听
下面讲解 Channel 要如何传递到 poll中以及 Poller 是如何封装 Channel 的.先看看Poller的数据成员
```cpp
private:
  void fillActiveChannels(int numEvents,
                           ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop* ownerLoop_;
  PollFdList pollfds_;
  ChannelMap channels_;
```
`ChannelMap`的 key 对应的就是文件描述符,因为文件描述符是唯一的,因此可以和 Channel 一一对应,`PollFdList`存放的是`struct pollfd`的数组,`struct pollfd.begin()`即可作为`poll`函数的第一个参数. 
要把 `Channel` 里的 fd 交给 poll 监听,就要根据 Channel 的 fd 及其注册的事件类型封装成 `struct pollfd`然后传给 `PollFdList pollfds_`,在 `poll` 返回后,根据 fd 又可以通过 `ChannelMap channels_`找到 `Channel`
 Channel对象 是通过`updateChannel`注册到 `PollFdList pollfds_` 和 `ChannelMap channels_`的
```cpp
void Poller::updateChannel(Channel* channel)
{
  assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->index() < 0) {
    // a new one, add to pollfds_
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size())-1;
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  } else {
    // update existing one
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd& pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == -1);
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    if (channel->isNoneEvent()) {
      // ignore this pollfd
      pfd.fd = -1;
    }
  }
}
```

Poller 在 EventLoop 的 `loop` 中不断地调用 poll 方法,并找到活跃的 fd, 根据 map 的记录再找到对应的 Channel,把 revents 赋值给 Channel,然后加入到 EventLoop 的 ActiveChannel_ 成员中, ActiveChannel_ 是一个`std::vector<Channel*>`,指向的是当期 active 的 Channel,在每一轮 loop 的 while 循环中, EventLoop 得到 activeChannels_ 后,就会调用列表中 Channel 的 handleEvent 函数,这个函数即根据当前 revents 调用用户注册的回调函数
来看看 `Poller` 对 `poll` 方法的封装
```cpp
Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0) {
    LOG_TRACE << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);
  } else if (numEvents == 0) {
    LOG_TRACE << " nothing happended";
  } else {
    LOG_SYSERR << "Poller::poll()";
  }
  return now;
}

//根据 poll 返回的结果填入参数activeChannels中
void Poller::fillActiveChannels(int numEvents,
                                ChannelList* activeChannels) const
{
  for (PollFdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd)
  {
    if (pfd->revents > 0)
    {
      --numEvents;
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);
      assert(ch != channels_.end());
      Channel* channel = ch->second;
      assert(channel->fd() == pfd->fd);
      channel->set_revents(pfd->revents);
      // pfd->revents = 0;
      activeChannels->push_back(channel);
    }
  }
}

```


由此可见每个IO事件都对应一个Channel,每个 Channel 都会传递给Poller ,Poller 会根据这些Channel 的fd 和要监听的对象去调用`poll`,Poller 和 Channel 是一对多的关系,而 EventLoop 和 Poller 是一对一的关系, EventLoop 让Poller 不断地调用poll 方法,拿到活跃事件后去促使 Channel调用其注册的回调函数

简而言之,以上的设计使得 用户得以对不同 fd ,以及不同的事件注册不同的自定义回调函数( fd + 事件类型才能确定回调函数,仅有fd 或仅有事件类型不行,这样就显得非常灵活),然后就是 while 循环调用 poll 方法看监听的事件是否到来,到来了,就根据事件类型去执行相应注册的回调函数,整个过程是单线程的,确保可以单线程的手段还是要`poll`这个系统调用

下面是loop的代码
```cpp
void EventLoop::loop()                                                                                                    
{  
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  quit_ = false;
   
  while (!quit_)
  {
    activeChannels_.clear();
    //调用系统的poll方法,然后将活跃时间添加进如 activeChannels_ 中
    poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent(); //根据事件调用其相应的回调函数
    } 
  }
   
  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}  

```

### 定时器
对应代码位置: https://github.com/chenshuo/recipes/tree/master/reactor/s02
这个目录下的所有代码实现了一个简单的定时器,测试文件对应 test4.cc,值得参考,
先看 test4.cc 代码
```cpp
// copied from muduo/net/tests/TimerQueue_unittest.cc

#include "EventLoop.h"

#include <boost/bind.hpp>

#include <stdio.h>

int cnt = 0;
muduo::EventLoop* g_loop;

void printTid()
{
  printf("pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
  printf("now %s\n", muduo::Timestamp::now().toString().c_str());
}

void print(const char* msg)
{
  printf("msg %s %s\n", muduo::Timestamp::now().toString().c_str(), msg);
  if (++cnt == 20)
  {
    g_loop->quit();
  }
}

//去处理 poll 事件的是Timequeue 而不是timers,timers用来存储到期时间和存储回调函数
// Timer 封装了任务时间,但不是每加一个Timer就注册一个时间,而是每次只根据离当前时间最近的Timer 使用 timer_create 注册
int main()
{
  printTid();
  muduo::EventLoop loop;
  g_loop = &loop;

  print("main");
  loop.runAfter(1, boost::bind(print, "once1"));
  loop.runAfter(1.5, boost::bind(print, "once1.5"));
  loop.runAfter(2.5, boost::bind(print, "once2.5"));
  loop.runAfter(3.5, boost::bind(print, "once3.5"));
  loop.runEvery(2, boost::bind(print, "every2"));
  loop.runEvery(3, boost::bind(print, "every3"));

  loop.loop();
  print("main loop exits");
  sleep(1);
}
```
首先讲解定时器思路,封装的思路如下:
* Timestamp, 对Epoch时间戳的封装
*　Timer，封装了 Timestamp 对象表示任务时间, repeat_ 对象表示是否重复执行(对于每x秒这种任务置为true)，inteval(在每x秒这种任务会用上)
* TimerQueue ,用 `std::set<std::pair<Timestamp,Timer*>>`作为存储 Timer 的数据结构,set 采用红黑树结构, `pair<Timestamp,Timer*>`而非直接使用`Timestamp`,是为了确保任务时间相同的 Timer 也可以存储在 TimerQueue 里

设计的运行流程概述,具体见代码:
一个线程里只有一个 Timerqueue 对象, Timerqueue 对象中用 set 存储了一系列的 Timer 定时任务. Timerqueue 在初始化时,会去注册成员函数 handleRead, 以后每次用`#include <sys/timerfd.h>`中的`timerfd_create` 和 `timer_settime`注册事件,poll 轮询事件到达时都会调用 `handleRead`这个回调函数,再由 `handleRead` 去调用 Timer 中的回调函数,Linux 中关于定时器的函数很多,比如`timer_create`使用的是 SIGNAL 来触发到期时间,因此不能驱动poll,select, 而使用`timerfd_create`则是用 fd 的方式,好处是 fd 的数量可以非常多,开销小,可以驱动 epoll,poll. 接下来先看看 `handleRead` 函数:

```cpp
//根据 now 构建 Timestamp,找到到达任务的时间,然后执行回调函数,注意不是根据当前时间去找到 now 对应的Timer,而是根据now去遍历
 timers_, 找出timers_ 中所有 < now 的 timer
//由此也可见,timers_ 中的 Timer 成员里面都保存了 Timestamp 对象, 但是不是所有 Timer 都会去调用 time_create 的,只有 timers_ 中最早的会去调用
void TimerQueue::handleRead()
{
  loop_->assertInLoopThread();
  Timestamp now(Timestamp::now()); //获取当前的时间戳
  readTimerfd(timerfd_, now); //主要用于调试,可pass
 
  //返回已经到期的 Timer,并从timers_ 中移除它们
  std::vector<Entry> expired = getExpi`std::set<std::pair<Timestamp,Timer*>>`red(now);
 
  // safe to callback outside critical section
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  { 
    it->second->run(); //调用其回调函数
  }

  //检查这些到期Timer是否设置了repeat_(runEvery),如果设置了,重新加入队列中,否则delete掉,接着
  reset(expired, now);
} 
  
//从 timers_ 中返回已经到期的 Timer,然后从 timers_中移除
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{ 
  std::vector<Entry> expired;
  //set 默认先按 first 排序,如果first相同,则比较second,reinterpret_cast<Timer*>(UINTPTR_MAX)保证了第二个值是最大的
  Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX)); //哨兵J
  //返回首个 > now 的迭代器,第一个未到期的Timer,lower_bound >=,为什么一定 > now? 是因为now相等时,sentry的value 取了(UINTPTR_MAX)
  TimerList::iterator it = timers_.lower_bound(sentry); 
  assert(it == timers_.end() || now < it->first); //未到期,所以不是<=
  //back_inserter 为 container 返回一个back_insert_iterator迭代器，这样，复制的元素都被追加到container的末尾了。
  //[begin(),it)的元素放入expired并被返回,it不会被拷贝进入expired
  std::copy(timers_.begin(), it, back_inserter(expired));
  timers_.erase(timers_.begin(), it);
  
  return expired;
} 
```
Timerqueue 中用一个对象 timers_ `(std::set<std::pair<Timestamp,Timer*>>类型)`来存储 Timer, Timer 重载了 `< `和`=` 运算符,使得可以被 set 按 Timestamp 按时间戳升序存储. 每次都会选取 timers_ 中最早的 Timer, 获取其存储的时间戳去注册事件,这里要明确的是,不管timers_ 中有多少个 timer, 某个时刻只会取任务时间距离当前最近的 timer 去注册事件,而不是所有timer都要同时去注册. 
每次往 timers_ 中加入新的 Timer ,都会用新的 timer 和当前 timers_ 中最早的 timer 做比较,如果新的 Timer 成员时间戳更早,则要用`timerfd_settime`重新注册.
当任务时间到达,被 poll 轮询得到时,调用 `handleRead`, handleRead 会根据当前的时间戳去检查 timers_ 中已经到期的 timers, 取出来并从 timers_ 中移除,分别执行取出 timer 的回调函数. 再检查这些 timer 是否设置了 repeat_ 属性的(用 runEvery 注册的),则计算这个 timer 的下次任务时间,然后重新 insert 进入 timers_ 中,加入 timers_ 时,依然要和 timers_ 中的 Timer 成员比较时间戳,重复前述过程.对于没有设置 repeat_ 的 timer,直接delete掉
