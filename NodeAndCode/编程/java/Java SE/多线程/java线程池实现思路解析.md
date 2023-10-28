### 尽量使用ThreadPoolExecutor
java.util.concurrent 包下提供了一个 ThreadPoolExecutor 类,它实现了 Excutor 接口,是一个线程池的实现类
当然也可以使用  Executors 提供的四种静态方法来创建线程池,这些静态方法其实最终也是调用了ThreadPoolExecutor,只不过帮我们默认设置了参数,比如:
ExecutorService cachedThreadPool = Executors.newCachedThreadPool();
  
但使用线程池时建议使用 ThreadPoolExecutor 而不建议用 Executors 去创建, 因为Executors 创建的线程池有如下隐患:
1. FixedThreadPool 和 SingleThreadPool: 允许的请求队列长度为 Integer.MAX_VALUE，可能会堆积大量的请求，从而导致 OOM。 
2. CachedThreadPool 和 ScheduledThreadPool: 允许的创建线程数量为 Integer.MAX_VALUE，可能会创建大量的线程，从而导致 OOM。  


### 线程池的实现思路:
#### 线程的开销
首先要明确使用线程池的目的,避免频繁创建线程和销毁线程的开销.在 java 中,这个开销就来自于 new Thread(). new Thread 中调用start()时会去调用 start0(),这是一个 native 方法,不是java实现的,不同的操作系统会有不同实现方式,比如linux 下最终就会调用 pthread_create, 这里就会有创建线程的开销  

#### 线程池采用的策略
由上面可知,要控制线程创建销毁的开销,就需要控制 Thread 对象的创建, ThreadPool 限制了 Thread 对象的创建,当线程池中的Thread 对象少于执行的任务数时,多出来的任务会被放入一个缓存队列中,具体要如何处理,等待多久才执行由我们设置.  
也就是说 ThreadPool 中创建了不超过设置数量的 Thread 对象,对于一个Thread对象,start成功后我们就申请到了线程资源,之后我们重复调用这个对象的 start 方法,就相当于一直在利用已经申请到的资源,这样就避免了线程的频繁创建开销了.  

由上面的理论也可以明白,我们要用 ThreadPoolExecutor 的 execute 方法去执行多线程时,传入 execute 的参数是实现了 Runnable 接口的对象而不是继承了 Thread 类的对象,因为如果传入一个继承了 Thread 类的对象,那我们在new这个对象的时候就相当于又创建了 一个 Thread,这时使用线程池就无意义了

阅读源码时的一些提示:
我们传给execute 的参数,最终会被线程池封装为 Worker 类,并添加到线程池里
```java
private Thread addThread(Runnable firstTask) {
    Worker w = new Worker(firstTask);
    Thread t = threadFactory.newThread(w);
    if (t != null) {
        w.thread = t;
        workers.add(w);
        int nt = ++poolSize;
        if (nt > largestPoolSize)
            largestPoolSize = nt;
    }
    return t;
}
```  
在调用 new Worker 时会到 ThreadFactory 去获得 Thread 对象, ThreadFactory 会去取已经创建好的能用的
Thread,这样就避免了Thread 的反复创建
```java
    private final class Worker extends AbstractQueuedSynchronizer implements Runnable {
        private static final long serialVersionUID = 6138294804551838833L;
        final Thread thread;
        Runnable firstTask;
        volatile long completedTasks;

        Worker(Runnable var2) {
            this.setState(-1);
            this.firstTask = var2;
            this.thread = ThreadPoolExecutor.this.getThreadFactory().newThread(this);
        }

        public void run() {
            ThreadPoolExecutor.this.runWorker(this);
        }

        //...
        
    }
```
var2 是我们传给excute 的参数,所以 firstTask 存储了我们要运行的对象
这里我们可以总结,线程池用一个 Worker 对象封装了我们要运行的对象(传给 execute 的参数), Worker 对象实现了 run 方法,并作为参数传入 Thread 中 (通过 getThreadFactory().newThread(this) 把当前 Worker对象传入), 当我们调用了 Thread 对象的 start()后,就会调用 Worker 对象的run(), Worker 的run 方法会去调用 runWorker 方法,而 runWorker 方法又会调用 firstTask(传给 execute 的参数,即我们最终目标要运行的对象) 的 run 方法  

#### Thread 对象和传入的Runnable对象
private Runnable target; 成员变量用于存储我们的目标对象,当我们把自定义的实现 Runnable 的类对象作为Thread构造函数参数传入时就由这个 target变量保存,调用 start 时就会找到这个target 去运行,而这个 target是可以替换的,因此同一个 Thread 对象是可以去执行不同的 Runnable 对象的,只要我们把 target 替换掉


```java

```
我们可以看看


推荐的源码解析博客:  
http://www.importnew.com/16797.html  
http://www.cnblogs.com/dolphin0520/p/3932921.html   
https://blog.csdn.net/tomatomas/article/details/51131576  

线程池里面的一些参数:
corePoolSize：核心池的大小,创建了线程池后，默认情况下，线程池中并没有任何线程，而是等待有任务到来才创建线程对象,默认情况下，在创建了线程池后，线程池中的线程数为0，当有任务来之后，就会创建一个线程去执行任务，当线程池中的线程数目达到corePoolSize后，就会把到达的任务放到缓存队列当中；  

maximumPoolSize：线程池最大线程数 

keepAliveTime：表示线程没有任务执行时最多保持多久时间会终止

unit：参数keepAliveTime的时间单位

workQueue： 线程池所使用的缓冲队列 

handler： 线程池对拒绝任务的处理策略,ThreadPoolExecutor中提供了一些策略
ThreadPoolExecutor.AbortPolicy()： 抛出java.util.concurrent.RejectedExecutionException异常 
ThreadPoolExecutor.CallerRunsPolicy(): 重试添加当前的任务，他会自动重复调用execute()方法 
ThreadPoolExecutor.DiscardOldestPolicy(): 抛弃旧的任务 
ThreadPoolExecutor.DiscardPolicy(): 抛弃当前的任务
