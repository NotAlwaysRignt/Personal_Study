#coding:utf-8
flask中context的解析
先上结论:当请求到来时,系统会调用Flask类的__call__方法(传入了envrion和responce),继而调用flask
的wsgi_app函数,生成一个Requestcontext类,因为environ和response是特定客户的,因此这
个Requestcontext实例也就是特定的.最后把这个Requestcontext压入栈中,在处理请求时,
若视图函数需要上下文环境时,就去访问栈顶,接着若要处理另一个客户的请求,则与该客户对应的
Requestcontext实例入栈,原来的出栈,这样就不会搞混了.


先看flask的__call__方法,它符合wsgi规范(第一个参数传environ,第二个参数传响应)
我们推测它会被wsgi注册,或在wsgi的函数中被调用
def __call__(self, environ, start_response):
        """Shortcut for :attr:`wsgi_app`."""
        return self.wsgi_app(environ, start_response)

Flask的wsgi函数 
def wsgi_app(self, environ, start_response):
    ctx = self.request_context(environ)     #上下文变量ctx被赋值为request_context(environ)的值  
    ctx.push()
    
ctx是一个RequestContext类

这个RequestContext类里面拥有的成员变量
#flask的ctx.py文件中
def __init__(self, app, environ, request=None):  
        self.app = app  
        if request is None:  
            request = app.request_class(environ) # wsgi_app的environ到了这里
        self.request = request  
        self.url_adapter = app.create_url_adapter(self.request)  
        self.flashes = None  
        self.session = None
        
ctx执行了push 方法,我们来看下它的push方法
def push(self):  
        top = _request_ctx_stack.top 
        if top is not None and top.preserved:  
            top.pop(top._preserved_exc)  
  
        app_ctx = _app_ctx_stack.top  
        if app_ctx is None or app_ctx.app != self.app:  
            app_ctx = self.app.app_context()  
            app_ctx.push()  
            self._implicit_app_ctx_stack.append(app_ctx)  
        else:  
            self._implicit_app_ctx_stack.append(None)  
  
        if hasattr(sys, 'exc_clear'):  
            sys.exc_clear()  
  
        _request_ctx_stack.push(self) #把自己 即ctx 压入了栈
        
这个push方法又执行了两次push,分别是
app_ctx.push() 
_request_ctx_stack.push(self)

先关注_request_ctx_stack,它把自己,即ctx压入栈

所以回顾一下,调用wsgi_app(self, environ, start_response)时
生成RequestContext对象ctx,并把ctx压入_request_ctx_stack中

看看_request_ctx_stack是什么
_request_ctx_stack = LocalStack()
它是werkzurg local.py文件中的LocalStack对象

也就是说,ctx把自己压入了LocalStack对象中

class LocalStack(object):  
      
        def __init__(self):  
            self._local = Local()  #这里用了Local类
      
      
        def push(self, obj):  
            """Pushes a new item to the stack"""  
            rv = getattr(self._local, 'stack', None)  
            if rv is None:  
                self._local.stack = rv = []  
            rv.append(obj)  
            return rv  
     
        @property  
        def top(self):  
            """The topmost item on the stack.  If the stack is empty, 
            `None` is returned. 
            """  
            try:  
                return self._local.stack[-1]  
            except (AttributeError, IndexError):  
                return None  

结合这个我们回过头来看RequestContext的push方法
def push(self):  
        top = _request_ctx_stack.top 
        #top拿到的是 return self._local.stack[-1] #即拿到的是列表末端的元素
        
        #self._local.stack是一个[]
        #rv = getattr(self._local, 'stack', None)  
            #if rv is None:  
                #self._local.stack = rv = []
                #此时rv和self._local.stack都会被赋值成空列表
                #但注意完成的操作不止这么多,因为self._local.stack=[]被重写过,详细见下
   
        
回顾 在ctx(RequestContext)的push操作中,有一步是将
_request_ctx_stack.push(self) #_request_ctx_stack是LocalStack类
而这个push是向self._local.stack中将ctx append进去
_local是一个Local对象,而_local.stack是一个[]
#如果Local对象_local中没有stack属性,就创建一个,并赋值为空列表
rv = getattr(self._local, 'stack', None)  
if rv is None:  
     self._local.stack = rv = []
至此,我们也看出,不过怎么封装,最后stack其实就是一个列表
总结,ctx.push(),会将自己压入到Local的stack(stack是[])中

看下_local.stack = rv = [],这里这个赋值是被重写过的
class Local(object):
def __setattr__(self, name, value):  
    ident = self.__ident_func__()  
    storage = self.__storage__  
    try:  
        storage[ident][name] = value  
    except KeyError:  
        storage[ident] = {name: value} 
        
所以,当你执行_local.stack的时候
你执行的是(__storage__是个字典)内容是{"线程标识符":{name:value},"线程"...  }形式
local.__storage__[self.__ident_func__()]["stack"]

总结,ctx.push()中,ctx自身最终会被push进入LocalStack对象内部的Local对象内部的一个字典中
其键值为stack

总结:一旦客户端一个请求到来,我们就把ctx压入栈中(此时栈中只有一个当前ctx对象,
这样,context对象看上去就像一个全局变量了)


