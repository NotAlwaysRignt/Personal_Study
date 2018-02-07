flask 的app.run干了什么

先上个总结,
app.run()中,默认选取'127.0.0.1'为主机IP,以及开启端口 5000
接下来调用werkzeug的run_simple函数,
这个函数根据'127.0.0.1'及端口 5000 生成了socket对象,并启动listen(监听是否有连接到来)和
accept(为每个客户都建立一个TCP连接)
上述操作都在构造一个BaseWSGIServer类的实例过程中完成:
srv = make_server(...)#返回BaseWSGIServer实例

在构造了BaseWSGIServer类的实例srv后,调用srv.serve_forever()
在srv.serve_forever()中,我们追溯到了_handle_request_noblock()这个函数
这个函数是在一个while循环中被调用的
ready = selector.select(poll_interval)
    if ready:
       self._handle_request_noblock()
就是说,serve_forever()使socket在不断监听,借助selectors模块完成高效的IO操作

一旦IO完成,就调用_handle_request_noblock(),在_handle_request_noblock()中,
self.process_request(request, client_address)
request和client_address是socket.accept返回的,
request可以调用request.recv(1024),request.sendall()等方法与对应的TCP客户端通信
client_address存储客户的IP等信息

self.process_request(request, client_address)中关键一步是调用
    self.finish_request(request, client_address)

这个函数内容很简单只有一句:self.RequestHandlerClass(request, client_address, self)
RequestHandlerClass 是参数的名字,实质就是 WSGIRequestHandler 类
WSGIRequestHandler 继承了 BaseHTTPRequestHandler 类,而 BaseHTTPRequestHandler
继承了 BaseHTTPRequestHandler ,

因此self.RequestHandlerClass(request, client_address, self)
中的(request, client_address, self)实际传给了BaseHTTPRequestHandler的__init__

BaseHTTPRequestHandler的__init__函数中调用了self.handle()
实质调用的是WSGIRequestHandler的handle()
而WSGIRequestHandler的handle()有一步:rv = BaseHTTPRequestHandler.handle(self)
BaseHTTPRequestHandler.handle中有一步:self.handle_one_request()
self.handle_one_request()是WSGIRequestHandler的方法 (函数不断地在两个类迂回啊)
里面有一步self.run_wsgi()
self.run_wsgi()中有一步 execute(self.server.app) (werkzeug serving.py)
self.server.app就是flask的实例,在构造 BaseWSGIServer(注意不是 WSGIRequestHandler) 
的时候就已经在其父类 BaseWSGIServer 的构造函数中完成赋值

而在execute(self.server.app)
def execute(app):
        application_iter = app(environ, start_response)
        ...
就在此处,调用了app这个实例,于是根据python的语法会调用flask的__call__方法

再回顾
srv = make_server(...)构造了BaseWSGIServer类的实例
期间完成了socket的创建,并开启listen 和accept

之后调用srv.serve_forever(),一旦有客户端TCP连接到来,并且读取完毕后
就对该客户生成一个WSGIRequestHandler类,在构造过程中便会调用之前
在BaseWSGIServer中得到的flask实例
BaseWSGIServer类只构造一次,它封装了flask实例,而WSGIRequestHandler类
会在每个客户请求到来时构造一次


先不看调试的部分,不传入debug=False,而采用debug=None
_host = '127.0.0.1'
_port = 5000
host = host or sn_host or _host
port = int(port or sn_port or _port)
#app.run()时默认host = '127.0.0.1',port = 5000
try:
    run_simple(host, port, self, **options)
    
run_simple在werkzueg的serving.py文件里
def run_simple(hostname, port, application, use_reloader=False,
               use_debugger=False, use_evalex=True,
               extra_files=None, reloader_interval=1,
               reloader_type='auto', threaded=False,
               processes=1, request_handler=None, static_files=None,
               passthrough_errors=False, ssl_context=None):
    if use_reloader:
        ...#由app.run()进入到这一步,use_reloder为默认的False
    else:
        inner()

看看这个inner干了什么
def inner():
    try:
            fd = int(os.environ['WERKZEUG_SERVER_FD'])
        except (LookupError, ValueError):
            fd = None
        srv = make_server(hostname, port, application, threaded,
                          processes, request_handler,
                          passthrough_errors, ssl_context,
                          fd=fd)
        #app.run()会传过来的hostname是'127.0.0.1',port为5000
        if fd is None:
            log_startup(srv.socket)
        srv.serve_forever()
核心的就是srv(意为server)
make_server函数返回的是:
return BaseWSGIServer(host, port, app, request_handler,
                              passthrough_errors, ssl_context, fd=fd)
这里host,port等参数采用了默认参数,如下:
(host=None, port=None, app=None, threaded=False, processes=1,
                request_handler=None, passthrough_errors=False,
                ssl_context=None, fd=None)
                
也就是说srv是一个BaseWSGIServer类的实例

class BaseWSGIServer(HTTPServer, object):
    def __init__(self, host, port, app, handler=None,
                     passthrough_errors=False, ssl_context=None, fd=None):
        if handler is None:
                handler = WSGIRequestHandler
        ...
        HTTPServer.__init__(self, (host, int(port)), handler)
        #这里host是app.run()传过来的'127.0.0.1',port是app.run()传过来的5000
        #HTTPServer在python3的http.server模块中(或python2.7的BaseHTTPServer中)
        #而handler是一个WSGIRequestHandler类,这个类很重要
        try:#BaseHTTPServer是python2.7的模块,python3中用http.server代替
            import SocketServer as socketserver
            from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
        except ImportError:#socketserver,http.server是python3的模块
            import socketserver
            from http.server import HTTPServer, BaseHTTPRequestHandler
    
回到srv上,srv是一个BaseWSGIServer类的实例,在生成BaseWSGIServer实例时完成的动作有:
1.拿到WSGIRequestHandler的地址(handler = WSGIRequestHandler),这个类很重要
2.HTTPServer.__init__(self, (host, int(port)), handler)


我们来看看HTTPServer.__init__(...)
#HTTPServer在http/server.py文件中
class HTTPServer(socketserver.TCPServer):

    allow_reuse_address = 1    # Seems to make sense in testing environment

    def server_bind(self):
        """Override server_bind to store the server name."""
        socketserver.TCPServer.server_bind(self)
        host, port = self.socket.getsockname()[:2]
        self.server_name = socket.getfqdn(host)
        self.server_port = port

显然,HTTPServer.__init__(self, (host, int(port)), handler)并未直接定义在HTTPServer中
所以应该是调用其父类TCPServer的__init__()方法

#TCPserver在socketserver.py文件中
看TCPserver干了什么
#这里server_address是('127.0.0.1',5000),RequestHandlerClass
class TCPServer(BaseServer):
#列出几个要用到的全局变量:
address_family = socket.AF_INET
socket_type = socket.SOCK_STREAM
request_queue_size = 5
def __init__(self, server_address, RequestHandlerClass, bind_and_activate=True):
        """Constructor.  May be extended, do not override."""
        BaseServer.__init__(self, server_address, RequestHandlerClass)
        #BaseServer.__init__中需要关注的一步是 self.server_address = server_address
        #以及self.RequestHandlerClass = RequestHandlerClass,RequestHandlerclass即
        #传入的handler,即servier.py中的WSGIRequestHandler类(是类,不是实例)
        
        self.socket = socket.socket(self.address_family,
                                    self.socket_type)
        #此处初始化了socket变量(终于来到最底层的socket了)
        if bind_and_activate:
            try:#这里完成了socket.bind()和socket.listen()
                self.server_bind()
                self.server_activate()
            except:
                self.server_close()
                raise



    def server_bind(self):
        #......省略不执行的
        self.socket.bind(self.server_address)
        #(self.server_address即传入的('127.0.0.1',5000))
        self.server_address = self.socket.getsockname()

    def server_activate(self):
        self.socket.listen(self.request_queue_size)#self.request_queue_size是5

回顾,在BaseWSGIServer的__init__函数中,调用HTTPServer.__init__
便启动了socket,监听app.run()默认指定的127.0.0.1和5000端口

在获得BaseWSGIServer的实例srv后,接下来看
srv.serve_forever()
 def serve_forever(self):
        self.shutdown_signal = False
        try:
            HTTPServer.serve_forever(self)#self指BASEServer这个实例
        except KeyboardInterrupt:
            pass
        finally:
            self.server_close()

正常情况下执行的应该是HTTPServer.serve_forever(self)
HTTPServerHTTPServer在python3的http.server模块中(或python2.7的BaseHTTPServer中)

回到HTTPServer看其定义,并没有serve_forever()这个方法
HTTPServer.serve_forever(self)应该是HTTPServer父类的方法    
即socketserver.py文件中的TCPServer类,顺着找进去,TCPServer类中也没有定义serve_forever
于是我们在TCPServer继承的父类,BaseServer中找寻
    class BaseServer:
    ....
    def serve_forever(self, poll_interval=0.5):
        ...
        self.__is_shut_down.clear()
        #__is_shut_down是threading.Event(),用于多线程管理,暂不做讨论,这并不影响往下阅读
        try:
        #_ServerSelector = selectors.SelectSelector
            with _ServerSelector() as selector:
                selector.register(self, selectors.EVENT_READ)
                #注册了一个文件对象,
                while not self.__shutdown_request:
                    ready = selector.select(poll_interval)
                    if ready:
                        self._handle_request_noblock()

                    self.service_actions()#在app.run中,这一步并不会干什么(pass)
        finally:
            self.__shutdown_request = False
            self.__is_shut_down.set()

这里我们用到了python的selectors模块,这是一个用于高效IO的模块
首先看看
selector.register(self, selectors.EVENT_READ)
根据官方文档register的介绍:register(fileobj, events, data=None)

    fileobj is the file object to monitor. It may either be 
an integer file descriptor or an object with a fileno() method.

第一个参数是一个文件描述符,或者是一个有fileno()方法的对象
(这时我们自定义的fileno()方法返回结果应是一个文件描述符)
我们传入的参数是self,即BaseServer,fileno()方法并没有定义在BaseServer中
而是在其子类TCPServer中(根据python的语法,self是会到子类找查找是否存在这个方法的)
    def fileno(self):
        """Return socket file number.
        Interface required by selector.
        """#源码中的注释里写得很明白,返回文件描述符,同时也是selector的接口
        return self.socket.fileno()
回顾一下
srv = make_server(...)中就已经生成了TCPServer实例
self.socket就是此前根据传入的'127.0.0.1'和端口5000生成的socket
在srv.serve_forever一步中,借助selectors模块,来实现高效的IO读取
ready = selector.select(poll_interval)#等待读取IO完毕

下面是很重要的函数了,我们的WSGI函数将在这里被调用
self._handle_request_noblock()

    def _handle_request_noblock(self):
        try:
            request, client_address = self.get_request()
        except OSError:
            return
        if self.verify_request(request, client_address):
            try:
                self.process_request(request, client_address)
            except Exception:
                self.handle_error(request, client_address)
                self.shutdown_request(request)
            except:
                self.shutdown_request(request)
                raise
        else:
            self.shutdown_request(request)

忽略异常的情况:
def get_request(self):
    return self.socket.accept()
    
之前我们的socket只是绑定了端口,并开始监听,读取传来的信息,而accpet则可以分辨并保存不同客户的连接
客户请求连接时，accept方法建立连接并返回服务器。accept方法返回一个含有两个元素的元组
(connection,address)。第一个元素connection是新的socket对象，服务器必须通过它与客户通信；
比如我们可以通过connection.recv(1024),接收1024个字符,connection.sendall(string)发送字符串
第二个元素 address是客户的Internet地址。

在BaseServer中
    def verify_request(self, request, client_address):
        return True
并且我们发现其父类TCPServer并没有重写这个方法,所以
if self.verify_request(request, client_address)看成if True:即可


看看 self.process_request(request, client_address),
    
    def process_request(self, request, client_address):
        self.finish_request(request, client_address)
        self.shutdown_request(request)

接下来是重点了,这里将会用到我们flask的wsgi方法:

    def finish_request(self, request, client_address):
        """Finish one request by instantiating RequestHandlerClass."""
        self.RequestHandlerClass(request, client_address, self)
        #这个self是BaseWSGIServer

看到源码的注释了吧, 通过实例化一个 RequestHandlerClass来完成一个请求
RequestHandlerClass就是根据WSGI协议的可调用的对象
回溯一下,RequestHandlerClass是传入的参数,实际就是传入的WSGIRequestHandler类,接下来我们要看
WSGIRequestHandler是什么
WSGIRequestHandler在werkzurg的serving.py文件中

class WSGIRequestHandler(BaseHTTPRequestHandler, object):
    ......看几个我们要关注的方法
    
首先WSGIRequestHandler中并没有定义__init__方法,所以,传入的request和client_address及self
参数应该是传到了父类BaseHTTPRequestHandler中
在serving.py我们可以看见
try:#python2版本
    from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
except ImportError:#python3版本
    from http.server import HTTPServer, BaseHTTPRequestHandler

我们在http/server.py中查找 BaseHTTPRequestHandler 的定义
class BaseHTTPRequestHandler(socketserver.StreamRequestHandler):
仍然没有找到__init__方法,再从socketserver.py文件中查找其父类StreamRequestHandler
class StreamRequestHandler(BaseRequestHandler):
仍没有__init__方法
再找BaseRequestHandler
class BaseRequestHandler:
    def __init__(self, request, client_address, server):
        self.request = request
        self.client_address = client_address
        self.server = server
        self.setup()
        try:
            self.handle()
        finally:
            self.finish()

request是前面拿到的一个socket对象,是与指定用户的一个TCP连接
可以执行request.recv(1024),request.sendall()等方法收发字符串
client_address包含了连接客户的IP地址等信息,server则是BaseWSGIServer这个类的实例
即werkzeug的run_simple中的srv变量

看看self.setup(),
BaseRequestHandler的setup方法被StreamRequestHandler的setup方法覆盖
在StreamRequestHandler中:
    rbufsize = -1
    wbufsize = 0
    disable_nagle_algorithm = False   
    
    def setup(self):
        self.connection = self.request
        if self.timeout is not None:
            self.connection.settimeout(self.timeout)
        if self.disable_nagle_algorithm:
            self.connection.setsockopt(socket.IPPROTO_TCP,
                                       socket.TCP_NODELAY, True)
        self.rfile = self.connection.makefile('rb', self.rbufsize)
        if self.wbufsize == 0:
            self.wfile = _SocketWriter(self.connection)
        else:
            self.wfile = self.connection.makefile('wb', self.wbufsize)
        
makefile方法:创建一个与该套接字相关连的文件
这里大概是初始化了TCP传输字符流的相关操作,暂不展开

看看self.handle(),根据python语法会调用最顶层子类的方法,因此
BaseRequestHandler 的 handle 方法被 WSGIRequestHandler 的 handle 方法覆盖
找到WSGIRequestHandler的handle方法(werkzeug serving.py)
    def handle(self):
        """Handles a request ignoring dropped connections."""
        rv = None
        try:
            rv = BaseHTTPRequestHandler.handle(self)
        #忽视异常情况
        except (socket.error, socket.timeout) as e:
            self.connection_dropped(e)
        except Exception:
            if self.server.ssl_context is None or not is_ssl_error():
                raise
        if self.server.shutdown_signal:
            self.initiate_shutdown()
        return rv


server.py文件中找到BaseHTTPRequestHandler

    def handle(self):
        """Handle multiple requests if necessary."""
        self.close_connection = True

        self.handle_one_request()
        while not self.close_connection:
            self.handle_one_request()
            
注意此处的 handle_one_request 将由 BaseHTTPRequestHandler 的子类
WSGIRequestHandler 的 handle_one_request覆盖
在werkzeug的serving.py中找到:
class WSGIRequestHandler(BaseHTTPRequestHandler, object):
    def handle_one_request(self):
        """Handle a single HTTP request."""
        self.raw_requestline = self.rfile.readline()
        if not self.raw_requestline:
            self.close_connection = 1
        elif self.parse_request():
            return self.run_wsgi()

这里我们只看 self.parse_request() 和 self.run_wsgi() 部分

parse_request解析一个HTTP请求,即对从TCP中获取的字符流进行切割,解析
得到方法('GET','POST',...),header等等,实质就是进行字符串的切割操作,若嫌太长可以暂且跳过

    def parse_request(self):
        """根据源码的解释,这个方法用来解析一个请求
        self.raw_requestline是从TCP流中获得的字符串,即一个HTTP请求
        结果存储在self.command, self.path, self.request_version以及
        self.headers.
        如果调用成功返回True,否则返回False
        """
        self.command = None  # set in case of error on the first line
        self.request_version = version = self.default_request_version
        self.close_connection = True
        requestline = str(self.raw_requestline, 'iso-8859-1')
        #在werkzurg中, self.raw_requestline = self.rfile.readline()
        #self.rfile.readline()获得的是TCP流中读取下来的字符串
        requestline = requestline.rstrip('\r\n')
        self.requestline = requestline
        words = requestline.split()
        if len(words) == 3:
            command, path, version = words
            try:
                if version[:5] != 'HTTP/':
                    raise ValueError
                base_version_number = version.split('/', 1)[1]
                version_number = base_version_number.split(".")
                
                if len(version_number) != 2:
                    raise ValueError
                version_number = int(version_number[0]), int(version_number[1])
                #...错误处理,不符合要求的协议会报错,此处代码省略

        elif len(words) == 2:
            command, path = words
            self.close_connection = True
            #...错误处理省略
        self.command, self.path, self.request_version = command, path, version

        # Examine the headers and look for a Connection directive.
        try:
            self.headers = http.client.parse_headers(self.rfile,
                                                     _class=self.MessageClass)
        except #不符合要求的HTTP头部,比如有过多的headers或过长的headers

        conntype = self.headers.get('Connection', "")
        #省略conntype的错误处理
        expect = self.headers.get('Expect', "")
        #...省略expect的错误处理

重点,终于来了,我们在底层代码中周旋了那么久,即将回到flask!
看看 self.run_wsgi(),在这个函数内部还定义了非常多的函数,这里我们挑其中几个来讲


    def run_wsgi(self):
        if self.headers.get('Expect', '').lower().strip() == '100-continue':
            self.wfile.write(b'HTTP/1.1 100 Continue\r\n\r\n')

        self.environ = environ = self.make_environ()
        headers_set = []
        headers_sent = []

        def execute(app):
            application_iter = app(environ, start_response)
            try:
                for data in application_iter:
                    write(data)
                if not headers_sent:
                    write(b'')
            finally:
                if hasattr(application_iter, 'close'):
                    application_iter.close()
                application_iter = None

        try:#代码的中间夹了这一步,注意这一步在run_wsgi(self)的作用域中,
        #在调用 run_wsgi 时会被调用
            execute(self.server.app)
        except ...
        
看看execute(self.server.app) 
execute 函数在 run_wsgi 内部定义,
第一句
application_iter = app(environ, start_response)

app是什么,是传入的 self.server.app,
self.server.app在哪里,先看看WSGIRequestHandler是怎么生成的

回溯到前面
在process_request(...)内执行的finish_request(...)中
self.RequestHandlerClass(request, client_address, self)
这个RequestHandlerClass传进来的函数参数的变量名,其实就是WSGIRequestHandler类


class WSGIRequestHandler(BaseHTTPRequestHandler, object):
      里面没有直接定义__init__方法,于是我们继续寻找其父类:
class BaseRequestHandler:
    def __init__(self, request, client_address, server):
        self.request = request
        self.client_address = client_address
        self.server = server

self.server = server,这个server即
self.RequestHandlerClass(request, client_address, self)中的self
这个self是BaseServer类,其子类是TCPServer类,而TCPServer的子类是HTTPServer
看看HTTPServer是怎么被调用的:
class BaseWSGIServer(HTTPServer, object):
    ...
    def __init__(self, host, port, app, handler=None,
                 passthrough_errors=False, ssl_context=None, fd=None):  
        ...
        HTTPServer.__init__(self, (host, int(port)), handler)
        self.app = app
        ...

所以self.server.app中的self.server其实就是BaseWSGIServer类的实例

而app是什么
再往上找
def make_server(host=None, port=None, app=None, threaded=False, processes=1,
                request_handler=None, passthrough_errors=False,
                ssl_context=None, fd=None):   
    return BaseWSGIServer(host, port, app, request_handler,
                              passthrough_errors, ssl_context, fd=fd)

继续往上找
在run_simple函数中定义了一个inner(),而inner()中有那么一步:
srv = make_server(hostname, port, application, threaded,
                          processes, request_handler,
                          passthrough_errors, ssl_context,
                          fd=fd)
这里的application是run_simple 传进去的                              
def run_simple(hostname, port, application,......)

谁调用了run_simple,回到最开头!就在flask的 app.run()里
try:
    run_simple(host, port, self, **options)
    
这个application,就是self,这个self是Flask的实例
因此self.server.app就是Flask的实例app

回到上面提到的execute方法()
application_iter = app(environ, start_response)
app是Flask的实例,而Flask中定义了__call__方法,
因此调用这个实例时会调用Flask的__call__方法

我们还要看看 environ 和 start_response是什么
    def run_wsgi(self):
        ...
        
        self.environ = environ = self.make_environ()
        ...
        def start_response(status, response_headers, exc_info=None):
            ...
environ 和 start_response的定义都能在run_wsgi(self)内部找到,
这里先不展开(详见flask_wsgi的讲解)

终于,我们弄清除了app.run()到底是怎么来到联络底层并来到Flask的__call__方法的了!
那么之后flask干了什么,请看另一个文档flask_wsgi的讲解
