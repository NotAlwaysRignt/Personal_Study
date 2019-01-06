runInLoop 是 EventLoop 对象的一个成员,它可以被跨线程并发调用,它接收一个函数对象参数`Functor`,`typedef boost:function<void()> Functor`
如果操作涉及到了修改线程间共享的变量,我们就可以把这个操作封装为`Functor`然后传给runInLoop.runInLoop 会保证在并发调用的情况下也会同步执行传入的 `Functor`回调函数对象
下面来介绍 runInLoop,先介绍为什么需要runInLoop,然后再介绍其实现

### receipe/reactor/s02 的问题
receipe/reactor/s02 中 EventLoop 是不可以跨线程调用的
不能跨线程调用的原因: 要找到答案,首先要思考跨线程调用带来的问题,是的,跨线程导致问题的场景是 race condition,因此跨线程共享 loop 带来的问题自然是对并行对 loop 进行读写时产生的问题,这里不只是 EventLoop 对象,还包括 EventLoop 内部的成员函数
有了以上思路,就可以找到以下可能出现的问题:

#### EventLoop::loop
EventLoop::loop
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
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent();
    }
  }
  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}
```

显然这个loop 方法不可以跨线程调用,原因很多,举个例子,假设一个线程通过`poller_->poll(kPollTimeMs, &activeChannels_)`获取到了`activeChannels_`,而另一个线程刚好调用了`activeChannels_.clear()`,这就会导致逻辑出错

#### TimerQueue 对象
EventLoop 里有一个 TimerQueue 对象负责管理定时器,这个对象是用 STL 容器来装载定时器 Timer 对象的,当多线程对其进行并行操作时,也会出错

###  receipe/reactor/s03 的解决方案
`EventLoop::loop`函数依然是线程唯一的,符合 One loop per thread 的思想,但定时器操作 runAfter 支持跨线程操作.
为了保障跨线程调用安全,即 TimerQueue 对象的安全,实现了 runInLoop 方法

所有操作共享变量的函数都封装到 `Functor` 交给 runInLoop 去运行,**即把要调用的函数作为回调function对象交给runInLoop,runInLoop会保证回调函数是同步运行而非并行执行,进而解决了 race condition 的问题**
这里 `Functor`是一个 function 类型`typedef boost::function<void()> Functor`
有了runInLoop 这个同步保证实现,后续所有共享变量的操作,都作为回调传递进来,比如 TimerQueue 里的 addTimer 方法
```cpp
TimerId TimerQueue::addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval)
{
  Timer* timer = new Timer(cb, when, interval);
  loop_->runInLoop(
      boost::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if (earliestChanged)
  {
    resetTimerfd(timerfd_, timer->expiration());
  }
}
```
`insert(timer)`这样的操作会涉及到对 TimerQueue 的写,因此不可以并行执行,所以交给 runInLoop 去回调运行

### runInLoop 实现机制
runInLoop 的核心功能:即便是跨线程调用,如果某个操作可能会导致 race condition ,而我们需要确保整个操作在多线程下也能同步进行,就把这个操作封装成 funtion 对象并交给 runInLoop 去处理,runInLoop 会保证内部的线程安全
使用场景:涉及到对一个多线程均可访问的线程共享变量读写时,都需要将读写操作交给runInLoop
下面看看 runInLoop 的实现
```cpp
//这个函数的作用直接执行参数回调函数cb,cb要传入这个函数再执行而不是直接执行的原因:保障能够在跨线程的情况下安全调用
void EventLoop::runInLoop(const Functor& cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(cb);
  }
}

//向pendingFunctors_里添加回调任务,跨线程调用的情况下会立即唤醒执行
void EventLoop::queueInLoop(const Functor& cb)
{
  {
  MutexLockGuard lock(mutex_);
  pendingFunctors_.push_back(cb);
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup(); //对wakeupFd_进行写event,使得wakeupChannel_执行其回到,wakeupChannel_并不是目的,目的是使poll从阻塞返回以执行后面的doPendingFunctors
  }
}
```
`pendingFunctors_`是 `std::vector<Functor>`类型的, 所有跨线程的回调都将放到这个  vector 里,当跨线程回调函数作为参数传入时,就会放到vector中并开始遍历调用它
触发vector开始遍历调用的是 wakeup 函数
```cpp
void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = ::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}
```
遍历 `vector<Functor>` 的函数同样是作为 function 回调绑定在一个 Channel 上.因此这里通过一个写来触发poll 使得 poll 返回并调用这个Channel 绑定的回调,事件和回调的绑定是在 EventLoop 的构造函数中完成的
```cpp
EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    callingPendingFunctors_(false),
    threadId_(CurrentThread::tid()),
    poller_(new Poller(this)),
    timerQueue_(new TimerQueue(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_))
{
  LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
  if (t_loopInThisThread)
  {
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread
              << " exists in this thread " << threadId_;
  }
  else
  {
    t_loopInThisThread = this;
  }
  wakeupChannel_->setReadCallback(
      boost::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();
}
```
`wakeupFd_`封装于wakeupChannel_中,**设置这个Channel对象的目的并不是为了用它的回调去操作 pendingFunctors,事实上它的回调并没有这样做,之所以要设置这个事件,只是为了让 poll 能够从阻塞中返回**
`wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));`
先看 handleRead ,可以它只是读了一个数据以相应wakeup,本身并不做其它操作
```cpp
//这里的 handleRead 响应响应一个读事件如wakeup(),目的是使 poll 从阻塞中返回,进而可以执行后面的 doPendingFunctors,目标回调并不在handleRead执行, 
void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}
```
我们看看`EventLoop::poll`
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
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent(); //执行相应的回调函数
    }
    doPendingFunctors();
  }

  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}
```
当`poller_->poll`返回并遍历`activeChannels_`后,会调用`doPendingFunctors()`,wakeup正是为了能够调用它
```cpp
void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
  MutexLockGuard lock(mutex_);
  functors.swap(pendingFunctors_);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors_ = false;
}
```

操作`pendingFunctors_`需要加锁,以保证并行调用不会出错
doPendingFunctor的设计也很巧秒,它用一个临时变量`functors`把`pendingFunctors_`拷贝出来,再去执行里面的`Functor`回调,这样可以使得临界区大大减小

还有一个问题是:为什么不把`dopendingFunctors`的操作放到wakeupChannel_中,而是要延后到遍历完 activeChannels_ 后再执行呢,理论上这是可行的,完全可以在 activeChannels_ 中先用 read 响应 `wakeup`方法中的 write ,然后再执行`dopendingFunctors`

原因还是为了尽量减少临界区时间,假设把`dopendingFunctors`操作绑定到`wakeupChannel_`的回调中,在并发量很大的情况下跨线程调用 runInLoop ,那么在 `poller->poll`返回并遍历`activeChannels_`时,会调用多次 wakeupChannel_ 的回调,每次调用 `dopendingFunctors`都会使用mutex锁,虽然通过将 vector `swap`到临时变量的方法缩小了临界区,但如果一次遍历`activeChannel`就要调用多次mutex,那开销也是不小的

因此,这里延后了`dopendingFunctors`的调用,`doHandleRead`只是响应了wakeup,把某段时间内跨线程的回调 Functor 都收集起来,然后再一口气调用.减少mutex的次数

这里的思路是:**减少了调用了mutex的频率,这会是跨线程Functor的调用延后,但如果并发量不大,这点延后开销很小,当并发量很大时,mutex的频率对性能影响很大,降低调用频率可以使性能显著提升**

以上就是对 runInLoop 设计思路的分析,再总结:多线程的困难之处就在于并发操作一个线程共享的变量 (race condition),而使用 mutex 锁进行同步会造成内核调用的开销.为了减少使用锁的频率,有两个重要的技巧:
* 批处理而非每一个单独处理,即置于`vector<Functor>`中,在轮询所有 poll 的 fd 结束后再遍历这个vector,减少加索频率
* 使用 swap 减小临界区,我们需要共享变量`pendingFunctors_`,但又不希望占有它太久,因为这会延长临界区,因此用swap把它拷贝到临时变量中,接着去读临时变量,这样就可以减小临界区了
