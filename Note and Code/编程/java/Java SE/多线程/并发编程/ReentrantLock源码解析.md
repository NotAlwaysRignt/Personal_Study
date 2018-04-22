java.util.concurrent.locks.ReentrantLock 是一个可重入(lock 多少次,就要 unlock 多少次)的乐观锁,它也采用了自旋锁的机制.
## 实现方法
ReentrantLock 维护了一个 state 变量,定义如下:  
private volatile int state;  
调用 lock 方法时,会去检查 state 是否为 0,若为 0 则设为1,并设置当前线程为锁独占的线程. 这个检查并设置的过程使用 CAS(Compare and Swap)实现,而 CAS 是由硬件实现的,会保证这个过程的线程安全.  
如果 state 不为 0 ,即冲突的情况:  
检查当前持有锁的进程是否为是本线程,如果是 state +1 (可重入就体现在此),然后还会检查是否溢出(state溢出后小于0),如果没有,就更新 state 的值.完成一次加锁操作.
如果发现当前线程不是持有锁的线程,那么会将该线程添加到等待队列中,这里不展开讲,然后调用 Thread.currentThread().interrupt();


非公平锁加锁部分源码解读,摘取核心部分
```java
public class ReentrantLock implements Lock, Serializable {
    private final ReentrantLock.Sync sync;
    //默认使用非公平锁
    public ReentrantLock() {
        this.sync = new ReentrantLock.NonfairSync();
    }

    
    public final void acquire(int var1) {
        if (!this.tryAcquire(var1) && this.acquireQueued(this.addWaiter(AbstractQueuedSynchronizer.Node.EXCLUSIVE), var1)) {
            selfInterrupt();
        }    

    //非公平锁的实现如下
    static final class NonfairSync extends ReentrantLock.Sync {
        NonfairSync() {
        }
        
        // compareAndSetState 会调用unsafe.compareAndSwapInt,这是个native方法,不在java实现
        // 最终会调用 CAS 操作,检查是否0,是0则更新为1,不是0就返回false 
        final void lock() {
            if (this.compareAndSetState(0, 1)) {  
                this.setExclusiveOwnerThread(Thread.currentThread());
            } else {
                this.acquire(1); //代码在ReentrantLock中
            }

        }

        protected final boolean tryAcquire(int var1) {
            return this.nonfairTryAcquire(var1);
        }
    }
    
    //NonfairSync extends ReentrantLock.Sync
    abstract static class Sync extends AbstractQueuedSynchronizer {
    /*
        
        private transient Thread exclusiveOwnerThread;  
        //transient表明不能持久化到磁盘中,只能在缓存里,因此也不能序列化
        
        protected final void setExclusiveOwnerThread(Thread var1) {
        this.exclusiveOwnerThread = var1;
        }
    
    */
    
        final boolean nonfairTryAcquire(int var1) {
            Thread var2 = Thread.currentThread();
            int var3 = this.getState(); 
            if (var3 == 0) {  //再获取一次state,若为0,就设置为1,并设置持有锁的线程为当前线程
                if (this.compareAndSetState(0, var1)) {
                    this.setExclusiveOwnerThread(var2);  //这为当前线程
                    return true;
                }
            } else if (var2 == this.getExclusiveOwnerThread()) {
                int var4 = var3 + var1;  //var1 为1,这里即可重入锁的体现
                if (var4 < 0) {  //溢出了,重入次数超过 int 最大值
                    throw new Error("Maximum lock count exceeded");
                }

                this.setState(var4);  //更新state的值
                return true;
            }

            return false;
        }

        protected final boolean tryRelease(int var1) {
            int var2 = this.getState() - var1;
            if (Thread.currentThread() != this.getExclusiveOwnerThread()) {
                throw new IllegalMonitorStateException();
            } else {
                boolean var3 = false;
                if (var2 == 0) {
                    var3 = true;
                    this.setExclusiveOwnerThread((Thread)null);
                }

                this.setState(var2);
                return var3;
            }
        }

    }
}

```
