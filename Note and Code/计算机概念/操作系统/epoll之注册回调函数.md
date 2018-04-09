epoll 之所以能如此快地让操作系统找到响应的文件描述符,就是因为它对每个文件描述符注册了一个回调函数,因此当事件到来时相应回调函数被调用,文件描述符就被添加到就绪队列里.  
当然,epoll 的开销也就是一部分也就是因为回调函数的注册  

回调函数的注册  
参考文章:https://yq.aliyun.com/articles/64858
(上面网址代码的字体颜色在我的浏览器上显示的是和背景色同色,导致看不见代码只看得见注释,用鼠标扫描后会看到代码,可以用开发者模式挑颜色)  
```C
//当poll醒来时就回调用该函数  
static void ep_ptable_queue_proc(struct file *file,wait_queue_head_t *whead,  
                poll_table *pt)  
{  
    //从注册时的结构中struct ep_pqueue中获取项epi  
    struct epitem *epi = ep_item_from_epqueue(pt);  
    /*//epitem的私有项,通过pwqlist来进行链接  
     *struct eppoll_entry  
     {  
        struct list_head llink;  
        void *base;  
        wait_queue_t wait;  
        wait_queue_head_t *whead;  
     }  
*/  
    struct eppoll_entry *pwq;//struct epitem的私有项，为每一个fd保存内核poll  
  
//为每一个等待的结构分配一项  
    if(epi->nwait >= 0 && (pwq = kmem_cache_alloc(pwq_cache,  
            GFP_KERNEL)))  
    {  
        //醒来就调用ep_poll_callback,这里才是真正意义上的poll醒来时的回调函数  
        init_waitqueue_func_entry(&pwq->wait,ep_poll_callback);  
        pwq->whead = whead;  
        pwq->base = epi;  
        //加入到该驱动的等待队列  
        add_wait_queue(whead,&pwq->wait);  
        //将等待链接也放入到epitem链表中去  
        list_add_tail(&pwq->llink,&epi->pwqlist);  
        epi->nwait ++;          
    } else {  
        epi->nwait = -1;  
    }  
}  
//当poll监听的事件到达时，就会调用下面的函数  
static int ep_poll_callback(wait_queue_t *wait,unsigned mode,int sync,void *key)  
{  
    int pwake = 0;  
    unsigned long flags;  
    struct epitem *epi = ep_item_from_wait(wait);  
    struct eventpoll *ep = epi->ep;  
      
    spin_lock_irqsave(&ep->lock,flags);  
    //判断注册的感兴趣事件   
//#define EP_PRIVATE_BITS      (EPOLLONESHOT | EPOLLET)  
//有非EPOLLONESHONT或EPOLLET事件  
    if(!(epi->event.events & ~EP_PRIVATE_BITS))  
            goto out_unlock;  
      
    if(unlikely(ep->ovflist != EP_UNACTIVE_PTR))  
    {  
        if(epi->next == EP_UNACTIVE_PTR) {  
            epi->next = ep->ovflist;  
            ep->ovflist = epi;  
        }  
        goto out_unlock;  
    }  
  
    if(ep_is_linked(&epi->rdllink))  
        goto is_linked;  
    //关键是这一句，将该fd加入到epoll监听的就绪链表中  
    list_add_tail(&epi->rdllink,&ep->rdllist);  
is_linked:  
    if(waitqueue_active(&ep->wq))  
        __wake_up_locked(&ep->wq,TASK_UNINTERRUPTIBLE   
            | TASK_INTERRUPTIBLE);      
    if(waitqueue_active(&ep->poll_wait))  
        pwake++;  
out_unlock:  
    spin_unlock_irqrestore(&ep->lock,flags);  
      
    if(pwake)  
        ep_poll_safewake(&psw,&ep->poll_wait);  
    return 1;  
}  
```  
ep_poll_callback 就是注册的回调函数啦!  
回调函数注册语句如下:
init_waitqueue_func_entry(&pwq->wait,ep_poll_callback);   

那么 init_waitqueue_func_entry 是什么东西,看看
参考文章: https://blog.csdn.net/qb_2008/article/details/6919987
```C
static inline void init_waitqueue_func_entry(wait_queue_t *q,  
                    wait_queue_func_t func)  
{  
    q->flags = 0;  
    q->private = NULL;  
    q->func = func;  
}  

```  
init_waitqueue_func_entry 是linux 内核的一个函数,
q->func = func;
我们的回调函数即 func,赋值给了 q, q 是wait_queue_t* 类型的变量
而 wait_queue_t 则涉及了 Linux 的等待队列机制  
https://www.douban.com/group/topic/79167871/  
等待队列机制是 Linux 开发驱动设备常用的机制,也是实现异步的核心,看到这里要有概念了,异步是否能实现,关键不是什么epoll,epoll它只是接口罢了,我们之所以能够异步,是设备能够支持异步.设备不可用时CPU不阻塞在那里等待设备可用,而是休眠进程去做其它事情,当设备可用时,设备会向 CPU 发送中断信号, CPU在中断程序中唤醒相应的进程继续执行,这才是异步的本质.在epoll里,注册的回调函数就是那个被唤醒的线程 
