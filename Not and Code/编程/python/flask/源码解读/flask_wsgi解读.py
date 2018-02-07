#coding:utf-8

本文讲述了调用flask的实例app(即调用Flask类的__call__方法)后发生了什么

首先客户端访问服务器时,会触发flask对象的__call__()方法,这是一个符合wsgi规范的函数
如何触发到call方法,可参考对app.run的讲解,当然,我们部署flask的时候不使用app.run()
但我们仍然会调用app这个实例,即调用__call__方法

先总结,调用__call__方法后,flask会先检查before_request中注册的函数,如果有就调用,如果此处
抛出了错误那就不会再调用视图函数,如果正常进行,下一步调用我们自定义的视图函数 
(@app.route中所定义的),视图函数的返回值会被封装成flask的Response类,之后再检查在
after_request中注册的函数,如果有就调用,最后将这个response类返回,之后对这个封装好的response
类中的信息发送给客户


(flask app.py)
def __call__(self, environ, start_response): # 根据WSGI协议，middleware必须是可调用对象
    return self.wsgi_app(environ, start_response)

我们先看看 envrion 和 start_response 是什么,既然是根据WSGI规范,那么不管app.run()还是
部署到服务器中, envrion 和 start_response 都有相同的格式

那我们看看app.run()里这两个参数的格式 
简单回顾下,app.run()调用了werkzeug的run_simple函数(serving.py文件中),
最后会掉用到run_wsgi这个函数,run_wsgi会执行excute函数,excute函数中调用了flask实例
,即调用了其__call__方法

envrion参数定义在run_wsgi函数中 
self.environ = environ = self.make_environ()

def make_environ(self):
    ...省略
    environ = {
        'wsgi.version':         (1, 0),
        'wsgi.url_scheme':      url_scheme,
        'wsgi.input':           self.rfile,
        'wsgi.errors':          sys.stderr,
        'wsgi.multithread':     self.server.multithread,
        'wsgi.multiprocess':    self.server.multiprocess,
        'wsgi.run_once':        False,
        'werkzeug.server.shutdown': shutdown_server,
        'SERVER_SOFTWARE':      self.server_version,
        'REQUEST_METHOD':       self.command,
        'SCRIPT_NAME':          '',
        'PATH_INFO':            wsgi_encoding_dance(path_info),
        'QUERY_STRING':         wsgi_encoding_dance(request_url.query),
        'REMOTE_ADDR':          self.address_string(),
        'REMOTE_PORT':          self.port_integer(),
        'SERVER_NAME':          self.server.server_address[0],
        'SERVER_PORT':          str(self.server.server_address[1]),
        'SERVER_PROTOCOL':      self.request_version
    }
    ...
    return environ

envrion是一个字典,上面存储了之前通过解析客户端请求获得而获得的各种信息
比如 请求的方法self.command(GET,POST...),用户端口等等

再看 start_response ,这个参数是一个函数,其功能就是返回响应给用户

同样定义在werkzeug的serving.py中
def start_response(status, response_headers, exc_info=None):
    if exc_info:
        try:
            if headers_sent:
                reraise(*exc_info)
        finally:
            exc_info = None
    elif headers_set:
        raise AssertionError('Headers already set')
    headers_set[:] = [status, response_headers]
    return write
这个我们稍后讨论


self.wsgi_app(environ, start_response)两个参数的内容,我们来看看wsgi_app这个函数
    
def wsgi_app(self, environ, start_response):
        ctx = self.request_context(environ)
        error = None
        try:
            try:
                ctx.push()
                response = self.full_dispatch_request()
            except ....
            return response(environ, start_response)
        #except中遇到return时，就强制转到 finally 中执行，在finally中遇到return时就返回
        finally:
            ...
我们假设一切都正常运行,忽略 except,finally 部分的内容
那么wsgi_app主要干了两件事,一是生成上下文环境变量ctx,并将其压入栈(ctx.push())
(ctx一个RequestContext类的实例,详见request和context源码解读)
另一个就是response = self.full_dispatch_request()
正如函数名字的意思,这是解析url的函数

url到视图函数的思路很简单,Flask采用和大多数框架相同的思路,即建立一个{url:view_function}的映射

我们先看看flask如何解析用户请求中的url
def full_dispatch_request(self):
    self.try_trigger_before_first_request_functions()#看有没有注册请求前要调用的函数
    try:
        request_started.send(self)
        rv = self.preprocess_request()
        if rv is None:
            rv = self.dispatch_request()
    except Exception as e:
        rv = self.handle_user_exception(e)
    return self.finalize_request(rv)  

self.try_trigger_before_first_request_functions()用于看有没有注册请求前要调用的函
和url映射无关,不想了解可跳过这个函数的讲解

try_trigger_before_first_request_functions()会执行我们自定义的在处理请求前要先执行的函数
这些方法我们可以在Flask的before_first_request()和before_request()中注册
如果我们没有定义这些函数,那它不会干任何事情

看看try_trigger_before_first_request_functions,实现思路并不难
    def try_trigger_before_first_request_functions(self):
        #Flask中,self._got_first_request = False,下面第一次遍历后会被置为True
        #下次再进来就return了
        if self._got_first_request:
            return
        with self._before_request_lock:
            #执行过一次后_got_first_request为True,再进入就return了
            if self._got_first_request:
                return
            for func in self.before_first_request_funcs:#遍历注册的函数
            #开头中找到self.before_first_request_funcs = []
                func()
            self._got_first_request = True
            #置为True,下一次再进入这个函数,就不会再遍历before_first_request_funcs了


request_started在signals.py中,这句主要用于信号通信,内部使用了blinker库,这里暂时不展开
rv = self.preprocess_request() 正如其方法的名字,是做一些预处理,包括注册蓝图等等,暂不展开
#rv正常情况下为None,但如果我们注册的before_request的处理函数有返回值(一般有返回值
#的情况是抛出了错误),那么rv不为None,视图函数就不会被执行

这里我们看重点
rv = self.dispatch_request()
这一步用于调用url请求对应的视图函数,也就是我们在app.route中注册的

def dispatch_request(self):
    req = _request_ctx_stack.top.request
    if req.routing_exception is not None:
        self.raise_routing_exception(req)
    rule = req.url_rule
    # if we provide automatic options for this URL and the
    # request came with the OPTIONS method, reply automatically
    if getattr(rule, 'provide_automatic_options', False) \
       and req.method == 'OPTIONS':
        return self.make_default_options_response()
    # otherwise dispatch to the handler for that endpoint
    return self.view_functions[rule.endpoint](**req.view_args)

_request_ctx_stack.top拿到的是RequestContext类的实例 (详见"flask_context解读")
req拿到的是 Flask Request类的实例(wrappers.py文件中)

self.view_functions是一个{}(详见route(url_rule)解读)
url_rule是客户请求的url, 在Request类中会将其解析客户的请求并将其保存在self.url_rule中
rule.endpoint即对应的视图函数的名称
view_args是客户请求的url中的变量

接下来看,rv保存了视图函数返回的结果,即我们要返回给客户端的内容,回忆一下,视图函数返回的结果
可以是字符串 return "<h1>Hello</h1>",也可以是 return render_template(...)

self.finalize_request(rv)

def finalize_request(self, rv, from_error_handler=False):
    response = self.make_response(rv)
    try:
        response = self.process_response(response)
        request_finished.send(self, response=response)
    except Exception:
        if not from_error_handler:
            raise
        self.logger.exception('Request finalizing failed with an '
                              'error while handling an error')
    return response


make_response视图函数的返回值rv转化成一个Response类的实例,它可以处理多种类型的rv
如果返回值rv本身就是 Response 实例，就直接使用它；如果返回值rv是字符串类型，
(比如我们视图函数的返回值可以是 "<h1>Hello</h1>",则rv就是这个字符串),就把它作为响应的 
body，并自动设置状态码和头部信息；如果返回值是 tuple，会尝试用 (response, status, headers) 
或者 (response, headers) 去解析。

def make_response(self, rv):
        status = headers = None
        if isinstance(rv, (tuple, list)):#只要rv是个list或tuple,都会是True
            len_rv = len(rv)
            
            if len_rv == 3:
                rv, status, headers = rv
            elif len_rv == 2:
                if isinstance(rv[1], (Headers, dict, tuple, list)):
                    rv, headers = rv
                else:
                    rv, status = rv
                '''
                省略错误处理,如果rv这个tuple或list的大小不符合要求,或者
                rv为None,都会报错
                '''
        #text_type在_compat.py里定义:PY2 = sys.version_info[0] == 2
        # (sys.version_info[0],若python版本为2.X则返回2,若python版本为3.X则返回3)
        #if not PY2:text_type = str(python3.x),else:text_type = unicode(python2.x)
        
        if not isinstance(rv, self.response_class):#self.response_class = Response
            if isinstance(rv, (text_type, bytes, bytearray)):
                rv = self.response_class(rv, status=status, headers=headers)
                status = headers = None
            else:
                try:
                    rv = self.response_class.force_type(rv, request.environ)
                except TypeError as e:
                    ...
        if status is not None:
            if isinstance(status, (text_type, bytes, bytearray)):
                rv.status = status
            else:
                rv.status_code = status
        if headers:
            rv.headers.extend(headers)
        return rv


response = self.make_response(rv) 中,response 为Response类的实例
(Response类在 flask 的 wrappers.py中可找到)
class Response(ResponseBase):
    default_mimetype = 'text/html'
内容及其简单,我们发现:
from werkzeug.wrappers import Request as RequestBase, Response as ResponseBase

由此可见,flask的 Response 继承了 werkzeug wrappers.py文件中的 Response 类,其绝大部分
功能都是werkzeu的 Response 类完成的

response = self.process_response(response)
这一步最主要的功能就是 调用 after_request 中注册过的函数
finalize_request的最后一步是 return response   
这个Response类的实例被返回,我们看它最后会被返回到哪:
在函数full_dispatch_request(self):中
    return self.finalize_request(rv)#

def wsgi_app(self, environ, start_response):
    ...     
    response = self.full_dispatch_request() 
    ...
    return response(environ, start_response)
    
def __call__(self, environ, start_response):
    return self.wsgi_app(environ, start_response)
    
调用flask的实例的__call__方法的是(以app.run为例)
(见app_run()解读)
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
        
Response类的实例给到了 application_iter 在这里没有把它再返回了,而是以迭代的方式 write(data)
这里我们也能预见这个动作就是向客户端发送最后的数据,这里就不再展开了
