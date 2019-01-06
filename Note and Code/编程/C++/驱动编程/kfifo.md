参考文章:
https://blog.csdn.net/linyt/article/details/53355355
https://www.cnblogs.com/Anker/p/3481373.html

> Linux 内核中的循环队列缓存,实现非常高效,简洁,其中利用的技巧也是非常值得学习的

### 用途
内核 kfifo 循环缓存队列,用于生产者消费者模型,生产者将数据放入队列,消费者从队列中取出
队列缓存长度一开始由开发者分配好,因为缓存做成了循环队列,可以充分使用缓存空间,比如如果缓存开头部分已经被消费者取出了,那么下次生产者在数据填满了缓冲区尾部后可以重新填入缓冲区开头,提高了使用效率

### 数据结构
```cpp
struct kfifo {
    unsigned char *buffer;     /* the buffer holding the data */
    unsigned int size;         /* the size of the allocated buffer */
    unsigned int in;           /* data is added at offset (in % size) */
    unsigned int out;          /* data is extracted from off. (out % size) */
    spinlock_t *lock;          /* protects concurrent modifications */
};
```
buffer是缓存区,这个是要我们外部自己分配好空间,然后赋值给 `struct kfifo->buffer`用来组成循环队列
**一个值得注意的地方是, in 和 out 都是 unsigned int 类型的**
这里使用了一个非常重要的技巧:对于 `unsigned int`,始终为正数,如果溢出,则从0开始,循环,如果得到了负数,则从unsigned int 的最大值开始往下减(设unsigend int 最大值为 MAX, 则负数 -x = MAX +1 -x ,x为大于0的正数)
对无符号数的这个特点也是整个队列实现的精妙之处所在
### 接口
```cpp
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size,
             gfp_t gfp_mask, spinlock_t *lock)
{
    struct kfifo *fifo;
    /* size must be a power of 2 */
    BUG_ON(!is_power_of_2(size));
    fifo = kmalloc(sizeof(struct kfifo), gfp_mask);
    if (!fifo)
        return ERR_PTR(-ENOMEM);
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;
    fifo->lock = lock;

    return fifo;
}
struct kfifo *kfifo_alloc(unsigned int size, gfp_t gfp_mask, spinlock_t *lock)
{
    unsigned char *buffer;
    struct kfifo *ret;
    if (!is_power_of_2(size)) {
        BUG_ON(size > 0x80000000);//0x80000000是int的最大值
        size = roundup_pow_of_two(size);
    }
    buffer = kmalloc(size, gfp_mask);
    if (!buffer)
        return ERR_PTR(-ENOMEM);
    ret = kfifo_init(buffer, size, gfp_mask, lock);

    if (IS_ERR(ret))
        kfree(buffer);
    return ret;
}
```
从`kfifo_init`可以看出,buffer,是要自己分配传入的,最后会组装成一个`struct kfifo`指针返回
在`kfifo_init`和`kfifo_calloc`中，`kfifo->size`的值总是在调用者传进来的 size 参数的基础上**向2的幂扩展，这是内核一贯的做法,这样做的好处是对 kfifo->size 取模运算可以转化为与运算,如`kfifo->in % kfifo->size` 可以转化为 `kfifo->in & (kfifo->size – 1)`**
另外值得一提的是,虽然 `kfifo_alloc`的第一个参数 size 是 unsigned int 类型的,但是传入size时不可以超过 int 的最大值,因为代码里面限制了

### put 和 get 操作
```cpp
static inline unsigned int kfifo_put(struct kfifo *fifo,
                const unsigned char *buffer, unsigned int len)
{
    unsigned long flags;
    unsigned int ret;
    spin_lock_irqsave(fifo->lock, flags);
    ret = __kfifo_put(fifo, buffer, len);
    spin_unlock_irqrestore(fifo->lock, flags);
    return ret;
}

static inline unsigned int kfifo_get(struct kfifo *fifo,
                     unsigned char *buffer, unsigned int len)
{
    unsigned long flags;
    unsigned int ret;
    spin_lock_irqsave(fifo->lock, flags);
    ret = __kfifo_get(fifo, buffer, len);
        //当fifo->in == fifo->out时，buufer为空
    if (fifo->in == fifo->out)
        fifo->in = fifo->out = 0;
    spin_unlock_irqrestore(fifo->lock, flags);
    return ret;
}


unsigned int __kfifo_put(struct kfifo *fifo,
            const unsigned char *buffer, unsigned int len)
{
    unsigned int l;
    //buffer中剩余空间的长度和当前要写入的长度len作比较
    len = min(len, fifo->size - fifo->in + fifo->out);
    /*
     * Ensure that we sample the fifo->out index -before- we
     * start putting bytes into the kfifo.
     */
    smp_mb();//内存屏障
    /* first put the data starting from fifo->in to buffer end 即从in,到缓冲区尾部*/
    l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    /* then put the rest (if any) at the beginning of the buffer,如果到尾部的缓冲区不够用,则要利用缓冲区头部开始到 out 的这段空间*/
    memcpy(fifo->buffer, buffer + l, len - l);

    /*
     * Ensure that we add the bytes to the kfifo -before-
     * we update the fifo->in index.
     */
    smp_wmb();
    fifo->in += len;  //每次累加，到达最大值后溢出，自动转为0
    return len;
}

unsigned int __kfifo_get(struct kfifo *fifo,
             unsigned char *buffer, unsigned int len)
{
    unsigned int l;
    //数据缓冲区中已经放入数据的长度
    len = min(len, fifo->in - fifo->out);
    /*
     * Ensure that we sample the fifo->in index -before- we
     * start removing bytes from the kfifo.
     */
    smp_rmb();
    /* first get the data from fifo->out until the end of the buffer */
    l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, fifo->buffer, len - l);
    /*
     * Ensure that we remove the bytes from the kfifo -before-
     * we update the fifo->out index.
     */
    smp_mb();
    fifo->out += len; //每次累加，到达最大值后溢出，自动转为0
    return len;
}
```
put 和 get 是实现非常巧妙,特别是 in 和 out 的赋值变化,这里做分析
#### put
在将一段长度为 len 的字符串放入缓存队列时,我们必须判断剩余的缓存队列空间是否足够容纳这个长度 len
`len = min(len, fifo->size - fifo->in + fifo->out);`
`fifo->size - fifo`是到缓冲区最尾部的可以空间,再加上开头处已经被读取,成为可用的缓冲区`fifo->out`,建议画个图,简单明了
使用min,表示,如果缓冲区足够大,则取len,否则只能取缓冲区剩余空间大小,赋值到len
接着`l = min(len, fifo->size - (fifo->in & (fifo->size - 1)))`
`fifo->in & (fifo->size - 1)`即`fifo->in % (fifo->size-1)`,因为前面`fifo->in`取了2的幂,所以可以这样运算
`fifo->size - (fifo->in & (fifo->size - 1))`是从in开始到这个缓冲区最尾部的空间,比较len和其大小.(建议画图)
如果 len 超过了这个空间,那么就利用从缓冲区的开头处的剩余空间(到out尾指),这就是循环队列的体现

`len = min(len, fifo->size - fifo->in + fifo->out);`可以放入的数据不会超过缓冲区大小,这在生产很快消费很慢的情况下有个容错保证,它会返回能够写入的长度,开发者需要将要写入的长度和返回值做比较,对于缓冲区不足的情况开发者要自己做处理

另一个比较有特点的是使用min来代替if,虽然说 min 里面有 if 判断,最后 if 还是没有省掉,但这样书写代码非常简洁,特别是在put部分,考虑缓冲区尾部已满要转入从头开始放入部分,连 if 都不用写:
`memcpy(fifo->buffer, buffer + l, len - l);`
如果len的长度没有超过缓冲区 in->end 的长度,那么 len-l 为0,这样就不会用到开头的部分

#### get
in 和 out 是`unsigned int`类型,它们都初始化为0,这里直接看`__kfifo_get`:
`len = min(len, fifo->in - fifo->out);`
len是要读取的长度,和当前缓冲区中存放的数据长度`fifo->in - fifo->out`作比较,保证读取的长度不会超过已有的数据长度,最后得到的是len是本次要读取的数据长度
`l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));`
本次要读取的数据长度 len 和out到 buffer->end 的这段长度作比较,赋值给l
这里解决的问题是:如果读取的长度 len 超过了 out 到 buffer 尾部的长度,那么还要从头开始读,如果len没有超过这个长度,则读剩下部分,对于这两种情况,代码用两句话搞定
```cpp
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    memcpy(buffer + l, fifo->buffer, len - l);
```
如果len长度没有超过 out 到 buffer 尾部的距离,那么第二次读时,len-l=0,相当于不会再从开头去读,代码非常简洁
in + le
读完 len 直接加 `fifo->out += len;`
### 总结
in 和 out 的关系值得系纠,这是这个设计的精华所在
##### 边界保证
看看 in 和 out 的边界如何保证(比如保证 out 不超过 in,out-int<=size)
out 增加是在 get 时处进行的`fifo->out += len;`
保证out不超过 in 的操作是 `len = min(len, fifo->in - fifo->out);`
len <=  fifo->in - fifo->out 因此 len+fifo->out<= fifo->in
所以 fifo->in - fifo->out >= len>=0
其它边界我们可以用此思路推出
##### 循环
循环队列的使用采用了取余数的方法,这里有几个思想非常巧妙:
* size 取2的幂,求余数变成了可以用与运算,详细见前面分析
* 取余运算可以和周期性联系起来
* 设 unsigned int 的取值为 x, x 取到 unsigned int 的最大值 MAX 时, x%size==size-1,下一步x+1,x变为0,此时 x%size == 0
要让这个成立,必须的保证是, size 必须是2的幂,因为 size有限制<=0x80000000,又因为它是2的幂,那它一定是 0x100..00的形式,而x 取到 MAX 时,一定是0x111...111的形式,因此取余数后可以确保值必为size-1