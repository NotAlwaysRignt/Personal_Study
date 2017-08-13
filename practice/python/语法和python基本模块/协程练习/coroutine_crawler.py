#coding:utf-8
from selectors import *
import socket
import re
import urllib.parse
import time


class Future:#管理生成器类
    def __init__(self):
        self.result = None
        self._callbacks = []

    def result(self):
        return self.result

    def add_done_callback(self, fn):#将回调函数加入到队列中
        self._callbacks.append(fn)

    def set_result(self, result):#给result赋值，把队列里的回调函数都执行了
        self.result = result     #此方法激活协程，让协程运行下去
        for fn in self._callbacks:
            fn(self)
        #fn事实上始终是同一个对象的方法，即task的step，但future可能在运行中有多个
    def __iter__(self):#python 2.7并不支持此函数
        yield self 
        return self.result#抛出异常的说明，并不是生成器的返回值


class Task:#Task对象只生成一次
    def __init__(self, coro): #传给coro的是fetcher.fetch()，即一个协程
        self.coro = coro
        f = Future()
        f.set_result(None)#让f里的所有回调函数执行(这里回调函数都是step)
        self.step(f)

    def step(self, future):
        try:
            next_future = self.coro.send(future.result)
            #这一步就是让fectch()内部往下运行
            #fetch()内嵌套创建了许多f，然而这些f最终都被yield到了fectch中
            #所以self.coro始终指向下一步

        except StopIteration:#生成器运行到了尽头也不让它抛异常
            return

        next_future.add_done_callback(self.step)
        #这一部后next_future这个变量我们就用不到了，但next_future指向的
        #对象依然存在，这个对象是connect函数中创建的f，它将在on_connected被调用后
        #执行f.set_result(None),此时因为之前next_future(f的引用)把step加入了
        #队列中，因此前一个future对象的step就会被执行
        #由此可见next_future是为了将前一个future的step加入队列而创建的临时变量
        #另外，整个程序在运行过程中其实只有两个future，一个是

urls_seen = set(['/'])#已经爬取的url
urls_todo = set(['/'])#未爬取的url
#追加了一个可以看最高并发数的变量
concurrency_achieved = 0
selector = DefaultSelector()
stopped = False


def connect(sock, address):#创建非阻塞connect
    f = Future()
    sock.setblocking(False)
    try:
        sock.connect(address)
    except BlockingIOError:
        pass

    def on_connected():
        f.set_result(None)#这一步会触发step，让下面的yield from f往下执行

    selector.register(sock.fileno(), EVENT_WRITE, on_connected)
    #一旦connect成功就会调用on_connected，即让生成器执行下去
    yield from f
    selector.unregister(sock.fileno())#connect成功后没必要再监视这个connect了


def read(sock):
    f = Future()

    def on_readable():
        f.set_result(sock.recv(4096))  #将读到的数据存入f的result变量中

    selector.register(sock.fileno(), EVENT_READ, on_readable)
    chunk = yield from f#此时先回到next_future.add_done_callback(self.step)
    #赋值给了next_future，再进入while循环
    selector.unregister(sock.fileno())
    return chunk


def read_all(sock):#将读取到的数据返回
    response = []
    chunk = yield from read(sock)
    while chunk:
        response.append(chunk)
        chunk = yield from read(sock)

    return b''.join(response)


class Fetcher:
    def __init__(self, url):
        self.response = b''
        self.url = url

    def fetch(self):#连接服务器，获取服务器下特定网页的内容，解析内容获取信息
        global concurrency_achieved, stopped
        concurrency_achieved = max(concurrency_achieved, len(urls_todo))

        sock = socket.socket()
        #注意这个connect不是sock.connect，而是上面自定义的connect函数，是个生成器
        yield from connect(sock, ('localhost', 3000))
        get = 'GET {} HTTP/1.0\r\nHost: localhost\r\n\r\n'.format(self.url)
        sock.send(get.encode('ascii'))
        self.response = yield from read_all(sock)
        #self.response存放着所有收到的数据
        self._process_response()#爬取所有从response获得的符合要求的url
        urls_todo.remove(self.url)#执行完上述任务后该url从url_todo中删除
        if not urls_todo:#当所有url_todo都爬取完毕后，结束while循环，停止爬虫工作
            stopped = True
        print(self.url)#打印url

    def body(self):#从response中提取并返回body的内容
        body = self.response.split(b'\r\n\r\n', 1)[1]
        return body.decode('utf-8')



    def _process_response(self):
        if not self.response:
            print('error: {}'.format(self.url))
            return
        if not self._is_html():#判断收到的数据response是否为html
            return
        urls = set(re.findall(r'''(?i)href=["']?([^\s"'<>]+)''',
                              self.body()))
        #从response中提取并得到body的内容，然后用正则表达式提取所有的url
        for url in urls:
            normalized = urllib.parse.urljoin(self.url, url)
            parts = urllib.parse.urlparse(normalized)
            if parts.scheme not in ('', 'http', 'https'):
                continue
            host, port = urllib.parse.splitport(parts.netloc)
            if host and host.lower() not in ('localhost'):
                continue
            defragmented, frag = urllib.parse.urldefrag(parts.path)
            #可以将url中#前后分离
            if defragmented not in urls_seen:#将url放入待处理和已抓取集合
                urls_todo.add(defragmented)
                urls_seen.add(defragmented)
                Task(Fetcher(defragmented).fetch())#然后开始用爬虫执行

    def _is_html(self):#判断收到的数据response是否为html
        head, body = self.response.split(b'\r\n\r\n', 1)
        headers = dict(h.split(': ') for h in head.decode().split('\r\n')[1:])
        return headers.get('Content-Type', '').startswith('text/html')


start = time.time()
fetcher = Fetcher('/')
Task(fetcher.fetch())

while not stopped:
    events = selector.select()#获取注册了的所有函数
    for event_key, event_mask in events: #event_mark是位掩码
        callback = event_key.data #callback是回调函数
        callback()

print('{} URLs fetched in {:.1f} seconds, achieved concurrency = {}'.format(
    len(urls_seen), time.time() - start, concurrency_achieved))



'''
协程是单线程的，回调是为了解决IO的阻塞问题

fetcher = Fetcher('/') 创建Fetcher对象，fecther对象只是拿到了url

fetcher.fetch() 拿到爬虫生成器，这个函数全部执行下来可以把当前对象对应的url爬取
并解析完相应，注意一个对象只对应一个url

Task拿到生成器对象，并在构造函数中的step函数中将fetch运行到connect处
(connect是全局函数)，connect开启socket连接，注册了on_connected函数
然后创建一个future对象这是future进行set_result什么也没做，然后把这个future对象
抛出给task中的step函数的局部变量next_future，next_futrue将当前的step函数
加入自己的[]中

上面就是初始化过程，我们获得了一个task对象，task对象管理着已经开启了
connect的fecther对象，注册的回调函数中有on_connected这个函数

接下来便开始了循环，根据注册的on_connected，一旦sock.connect启动，便会让生成器往
下执行,(生成器f在Fectcher中),这时f中的_callbacks队列并没有东西，不管它，协程往下
运行会爬取第一个首页的内容，解析并获取所有url

我们来看看程序运行到什么地方会yield

1.sock = socket.socket()
yield from connect(sock, ('localhost', 3000))

selector.register(sock.fileno(), EVENT_WRITE, on_connected)
等待连接时会先yield f
协程将在连接成功后被回调函数启动

yield时，程序会继续在while not stopped+for循环中找有没有已经可以回调的函数

2.sock.send(get.encode('ascii'))
self.response = yield from read_all(sock)

chunk = yield from read(sock)

selector.register(sock.fileno(), EVENT_READ, on_readable)
chunk = yield from f

def on_readable():
    f.set_result(sock.recv(4096))
其实注册的回调函数都是让当前停止的程序进行下去



1.协程是单线程的！它只是某处停下来，然后返回到上层，上层再往下执行
2.本例中执行了send的最后都拿到了一个f
'''







