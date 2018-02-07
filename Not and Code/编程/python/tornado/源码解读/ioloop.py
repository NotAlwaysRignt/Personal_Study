#tornado 1.2的源码
tornado 的 ioloop 是其支持高并发的核心
源码注释中给了例子如下：

import errno
import functools
import ioloop
import socket

def connection_ready(sock, fd, events):
    while True:
        try:
            connection, address = sock.accept()
            #connection是新的socket对象
            #address 是tuple (host,port)，如('127.0.0.1',5000)
        except socket.error, e:
            if e.args[0] not in (errno.EWOULDBLOCK, errno.EAGAIN):
                raise
            return
        connection.setblocking(0)
        handle_connection(connection, address)

#创建一个socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setblocking(0)
sock.bind(("", port))
sock.listen(128)

io_loop = ioloop.IOLoop.instance()
#偏函数，callback即connection_ready，只不过第一个参数sock默认使用上面创建的sock对象
callback = functools.partial(connection_ready, sock)
#这一步把ioloop实例 和 socket连接起来
io_loop.add_handler(sock.fileno(), callback, io_loop.READ)
io_loop.start()

io_loop = ioloop.IOLoop.instance()
找到 IOloop类，这个类没有继承其它自定义的类
class IOLoop(object):

找到instance方法

    @classmethod
    def instance(cls): 
        if not hasattr(cls, "_instance"):
            cls._instance = cls()
        return cls._instance

在类中使用了 @classmethod 的方法，则这个方法的第一个参数就是这个类的地址，
所以cls为 class IOLoop 
cls._instance = cls()等价于 IOLoop._instance = IOLoop()
所以
io_loop = ioloop.IOLoop.instance() 其实就是等价与 io_loop = ioloop.IOLoop()

接下来看io_loop.add_handler(sock.fileno(), callback, io_loop.READ)，
这一步将socket和ioloop联系起来，sock.fileno()返回sock的文件描述符，
callback即上面的 connection_ready，也是add_handler的参数handler

events是一个常数，这个常数是epoll提供的api要用的 READ = _EPOLLIN = 0x001，这里我们并不打算深入了解
epoll，我们暂时只要知道如何使用epoll的api即可


    def add_handler(self, fd, handler, events):
        """Registers the given handler to receive the given events for fd."""
        self._handlers[fd] = stack_context.wrap(handler)
        self._impl.register(fd, events | self.ERROR)

self._handlers = {} # 注意python字典的key 不一定是要str类型的，int类型也可以做key

stack_context是一个模块 from tornado import stack_context
在stack_context找到 wrap函数，参数fn是我们传入的handler即 callback，即使自定义的 connection_ready
参数 events 为传入的 io_loop.READ

def wrap(fn):
    '''Returns a callable object that will resore the current StackContext
    when executed.

    Use this whenever saving a callback to be executed later in a
    different execution context (either in a different thread or
    asynchronously in the same thread).
    '''
    if fn is None:
      return None
    def wrapped(callback, contexts, *args, **kwargs):
        if (len(_state.contexts) > len(contexts) or
            any(a[1] is not b[1]
                for a, b in itertools.izip(_state.contexts, contexts))):
            # contexts have been removed or changed, so start over
            new_contexts = ([NullContext()] +
                            [cls(arg) for (cls,arg) in contexts])
        else:
            new_contexts = [cls(arg)
                            for (cls, arg) in contexts[len(_state.contexts):]]
        if len(new_contexts) > 1:
            with contextlib.nested(*new_contexts):
                callback(*args, **kwargs)
        elif new_contexts:
            with new_contexts[0]:
                callback(*args, **kwargs)
        else:
            callback(*args, **kwargs)
    if getattr(fn, 'stack_context_wrapped', False):
        return fn
    contexts = _state.contexts
    result = functools.partial(wrapped, fn, contexts)
    result.stack_context_wrapped = True
    return result


传入的connection_ready（即fn）并没有 stack_context_wrapped，
所以getattr(fn, 'stack_context_wrapped', False)得到False，
_state()在stack_context定义,是一个全局的函数
class _State(threading.local):
    def __init__(self):
        self.contexts = ()
_state = _State() 
_state是一个继承了 threading.local 的_State类 的实例，同时也是全局存在的

接下来又创建了一个偏函数 result，其实就是在wrap函数内定义的 wrapped函数，并且第一个参数和第二个参数
绑定为 fn(即connection_ready)和 contexts (就是一个tuple() )，这里先不管这个result 的功能
还给result 绑定了一个 stack_context_wrapped 函数，然后把result返回

result返回后给了 self._handlers[fd]，也就是以文件描述符为key，键值为result这个偏函数

再看
self._impl.register(fd, events | self.ERROR)
这个_impl在IOLoop类的 __init__中定义
    def __init__(self, impl=None):
        self._impl = impl or _poll()
    我们知道创建IOLoop时，例子中我们是没传入参数的 io_loop = ioloop.IOLoop.instance()
    
也就是说这里 self._impl = _poll()
这个_poll()在不同平台下会有不一样的定义，在Linux并且支持 epoll的环境下(我们就讨论这种情况)
    try:
        # Linux systems with our C module installed
        import epoll
        _poll = _EPoll
再看看 _EPoll：

class _EPoll(object):
    """An epoll-based event loop using our C module for Python 2.5 systems"""
    _EPOLL_CTL_ADD = 1
    _EPOLL_CTL_DEL = 2
    _EPOLL_CTL_MOD = 3

    def __init__(self):
        self._epoll_fd = epoll.epoll_create()

    def fileno(self):
        return self._epoll_fd

    def register(self, fd, events):
        epoll.epoll_ctl(self._epoll_fd, self._EPOLL_CTL_ADD, fd, events)

    def modify(self, fd, events):
        epoll.epoll_ctl(self._epoll_fd, self._EPOLL_CTL_MOD, fd, events)

    def unregister(self, fd):
        epoll.epoll_ctl(self._epoll_fd, self._EPOLL_CTL_DEL, fd, 0)

    def poll(self, timeout):
        return epoll.epoll_wait(self._epoll_fd, int(timeout * 1000))
        
        
就是对epoll的封装，在python3中，要使用epoll可以如下方法使用
import select
epoll = select.epoll()
我们看见  _EPoll类封装了epoll.epoll_create()  epoll.epoll_ctl epoll.epoll_wait
这三个就是内核中epoll提供的api，具体使用方法看epoll使用说明
简而言之，epoll就是可以通过它的api epoll_wait()，在有连接到来并且可读时直接把可读的文件描述符返回
C语言中大概使用方法是(省略细节)
    struct epoll_event ev,events[20]; #创建结构体和一个结构体数组
    epfd = epoll.epoll_create(65535) #创建一个epoll的文件描述符
    listenfd = socket(AF_INET, SOCK_STREAM, 0); #创建一个socket 
    #此处省略对socket一些属性的设置过程
    
    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;
    
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev); #注册新的fd(此处为listenfd)到epfd中
    
    while(true):
    {
        nfds=epoll_wait(epfd,events,20,-1); 
        #参数分别为 epoll的 fd，struct epoll_event数组events，events数组的大小，超时时间
        #当有可用的fd时
        for(i = 0; i < nfds; i++) {
        //events[i].data.fd是epoll事件中弹出的套接字，即新的连接创建的socket的fd
        //接收连接
        sock_new = accept(events[i].data.fd，...);//accept其它参数这里省略了
        if(sock_new < 0) {
            fprintf(stderr, "接收客户端连接失败/n");
            continue;
        }
    }
    
理一下
IOLoop 在创建实例时，__init__函数就就有
self._impl = _poll()
在支持epoll的Linux系统中即等价于self._impl = _EPoll()
而在 _EPoll的__init__函数中 self._epoll_fd = epoll.epoll_create()
也就是说在实例化 IOLoop时，就已经初始化了 封装了epoll各种方法的类

所以，调用
self._impl.register(fd, events | self.ERROR)
时，对照上面epoll使用的例子，相当于已经完成了 
epfd = epoll.epoll_create(65535)
epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev); 
#注意python 中epoll的用法和 C中有所不同，不论是传入的参数或者是返回值
#比如epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev)
#python中最后一个参数不用传struct epoll_event类型的参数
#epoll_wait返回一个列表，里面的每个元素是tuple(fd,event)，
#fd是客户端socket的文件描述符,event是事件(int类型)
其中 listenfd 即是程序示例中我们传入的 sock.fileno()，events即传入的

回到程序示例中总结一下：
io_loop.add_handler(sock.fileno(), callback, io_loop.READ)
在创建一个服务器的socket后把文件描述符传入这个函数，可以将它传到epoll的api里面去，这样当有客户端
连接时就可以很快地拿到可以读取的文件描述符了

接下来看
io_loop.start()

里面是一个while循环，根据我们一开始给的例子，里面有很多判断是false，把那些不会被执行的程序段跳过
我们选取核心部分，代码的解析写在注释上

def start(self):
        """Starts the I/O loop.

        The loop will run until one of the I/O handlers calls stop(), which
        will make the loop stop after the current event iteration completes.
        """
        #...
        self._running = True
        while True:
            # Never use an infinite timeout here - it can stall epoll
            poll_timeout = 0.2

            #...
            try:
                event_pairs = self._impl.poll(poll_timeout) 
                #返回的是列表，列表里每个元素都是一个元组(fd,event_code)
            except Exception, e:
            #...
            
            self._events.update(event_pairs) 
            #_events是一个dict, 这句的作用是，例如 events = {1:100} 
            # event_pairs = [(2,121),(3,134)],那么 _events.update(event_pairs)后
            #_events = {1:100, 2:121, 3,134}
            while self._events:
                fd, events = self._events.popitem()
                #字典的popitem 将返回一个tuple (key, key_value)
                try:
                    self._handlers[fd](fd, events)
                    #_handlers是一个dict，key为 每个sock的fd，key_value是对应的回调函数
                except #...
        # reset the stopped flag so another start/stop pair can be issued
        # ...
        
理一下上面见代码的过程
在本例中(即一开始给的那个例子)
一开始我们创建了一个服务器的socket 将服务器socket的文件描述符(记为srv_fd)
我们在初始化 self._impl时已经创建了 epoll的文件描述符，
并将srv_fd通过self._impl.register注册进了epoll里。
当有客户端连接时，srv_fd上有事件发生，于是 self._impl.poll()就会返回
返回值是[(服务器的fd,event)],我们知道了服务器的socket的fd可读，于是
之前我们已经根据srv_fd作为key，键值为回调函数connection_ready偏函数(第一个参数sock，绑定为
最开始创建的服务器socket)，存放在self._handlers的字典中

于是在self._handlers[fd](fd, events)一步中调用这个函数，(第一个参数已经被我们绑定为服务器sock了)
于是我们用 sock.accept可以获得服务器与这个客户建立的新的socket对象，以及客户端的host，端口等信息
后续 handle_connection(connection, address)是并没有实现的方法，我们拿到了新的socket对象以及
address，就可以进行进一步操作，比如我们可以取得新的socket对象的文件描述符，然后也将它添加进
self._handlers字典中，并给它对应的回调函数，并把这个文件描述符也注册进epoll中

记住一开始服务器创建的socket(记为sever_socket) 和 之后 客户端连接服务器创建的socket关系

服务器始终用这个server_socket 监听 客户的连接，当有客户连接时
通过 server_socket.accept(),就可以获得新的client_socket,我们可以对这个client_socket进行读写
与客户通信，每个client_socket只用于服务器和一个客户的通信，
而server_socket一直在工作，用于监听是否有其它客户连接。

这就是ioloop的高效所在，弄清楚后其实也没有什么神秘，最底层还是socket，只是封装并利用了epoll模块，
当有非常多的客户进行连接时，epoll可以很好地管理这些socket的文件描述符，它可以即时获得有事件发生可操作的
文件描述符，而不用对每个文件描述符进行扫描,使得socket的通信变得更加高效。

