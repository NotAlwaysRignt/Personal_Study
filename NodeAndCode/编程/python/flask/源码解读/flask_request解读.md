#coding:utf-8
request 与上下文环境context密切相关,若不了解context,先看文档"flas_context解读"

    先上结论,request是LocalProxy的实例,当使用request的成员时,比如使用request.form或
request.args等,因为LocalProxy的__getattr__方法被重写,所以,最终__getattr__最终返回的是
ctx.request,(ctx在调用request的时候已经存在栈中,原因见文档flask_context解读)

ctx是RequestContext类的实例,它是一个上下文环境,而ctx.request中的request是
flask中Request类的实例(wrappers.py文件中),它包含了解析HTTP报头等许多方法,所以访问
request.form,其实就是访问ctx.request.form  因为每个请求到来时,ctx通过出入栈会自动
被更新为当前的上下文环境(详见文档),因此request其实也就是对应当前客户端的request


request = LocalProxy(partial(_lookup_req_object, 'request'))
首先看看_lookup_req_object是个什么东西  
```python
def _lookup_req_object(name):  
    top = _request_ctx_stack.top  
    if top is None:  
        raise RuntimeError(_request_ctx_err_msg)  
    return getattr(top, name) 
```   
_request_ctx_stack.top对应的是什么,来看看(_request_ctx_stack是LocalStack类的对象)
```python
class LocalStack(object):
@property  
        def top(self):  
            try:  
                return self._local.stack[-1]  
            except (AttributeError, IndexError):  
                return None  
```               
top拿到的是_local.stack[-1]即[]的最后一个元素(栈顶)(_local.stack是[])
而_local.stack[-1]存储的是ctx (为什么是ctx见),
所以 top=ctx

接着
return getattr(top, name)
第二个name参数是字符串,比如getattr(c,"value"),等价与执行c.value.  
getattr是python的一个内置函数,此句作用相当于top.name 

综上所述 _lookup_req_object("request")等价于得到ctx.request

那LocalProxy是干什么的?
```python
class LocalProxy(object):
   def __init__(self, local, name=None):                                   
        object.__setattr__(self, '_LocalProxy__local', local) 
         #等价于 self._LocalProxy__local=local                               
        object.__setattr__(self, '__name__', name)                          
        if callable(local) and not hasattr(local, '__release_local__'):     
            # "local" is a callable that is not an instance of Local or     
            # LocalManager: mark it as a wrapped function.                  
            object.__setattr__(self, '__wrapped__', local)
```
requst是LocalProxy对象,在__init__函数中
request._LocalPoxy_local = _lookup_req_object(name = "request")
即request._LocalPoxy_local是一个函数

LocalProxy重写了  __getattr__方法  
```python
def __getattr__(self, name):                                            
         if name == '__members__':     
             return dir(self._get_current_object()) 
         return getattr(self._get_current_object(), name)
         

def _get_current_object(self):
        if not hasattr(self.__local, '__release_local__'):
        #判断self.__local对象中是否有__release_local__成员
            return self.__local()
        try:
            return getattr(self.__local, self.__name__)
        except AttributeError:
            raise RuntimeError('no object bound to %s' % self.__name__)
 
```
问题来了,self.__local是个什么东西,
看回LocalProxy类的__init__方法  
```python
 def __init__(self, local, name=None):
        object.__setattr__(self, '_LocalProxy__local', local)
```
object.__setattr__(self, '_LocalProxy__local', local)  等价于self.__local=local
此处涉及了python私有成员变量的命名规则问题
类中成员变量加双下划线,如类A中有self.__value变量,外部访问时
用(a是A的实例)a.__value是访问不了的,要用a._A__value才可以,这里便是类似这种情况

再看这个__local,其实就是一个偏函数
partial(_lookup_req_object, 'request')
显然,这是一个函数,在LocalProxy的函数
def _get_current_object(self):
    #这里是判断代理的对象是不是一个werkzeug的Local对象,这里request显然不是
    if not hasattr(self.__local, '__release_local__'):
            return self.__local()#此处会被执行
     ...后面try部分不会被执行

所以最后我们从 _get_current_object(self)中获得的是
self.__local()的返回值,即_lookup_req_object("request")的返回值,即
ctx.request  #ctx是RequestContext实例,它是一个上下文环境
#当一个请求到来时,根据environ参数,ctx入栈顶
而当我们在flask中调用了request.XXX时
触发了request(LocalProxy实例)的__getattr__方法
最终获得了ctx.request.XXX (注意ctx.request和flask全局的request是不同的类)

接下来继续看看ctx.request是什么东西

看看类RequestContext  
```python
def __init__(self, app, environ, request=None):  
        ...
        if request is None:  
            request = app.request_class(environ) # wsgi_app的environ到了这里
        self.request = request 
        ...
```
request_class(evrion)根据wsgi里的environ创建出来的

```python        
#flask app.py
class Flask(_PackageBoundObject):
    def request_context(self, environ):
        return RequestContext(self, environ)
```    
传入RequestContext的第一个参数是self
我们使用flask时 就是app=Flask(),
所以RequestContext拿到的第一个参数就是这个app
由此我们可以知道RequestContext的__init__中app.request_class(environ)
request_class是Flask类的方法,
而eviron也是Flask类中request_context(self, environ)这个符合wsgi规范的函数的参数
即environ其实是wsgi里面的environ

回到Flask类的代码  
class Flask(_PackageBoundObject):
    request_class = Request
 request被定义成了类的成员变量
 即当使用app=Flask()时,app.request_class可以访问到它,类中self.request_class可以访问到它
 
看看这个Request类
这里我们先明确一点,当我们使用全局的request时,拿到的其实时ctx.request,而这个
ctx.request就是这个Request类,那些LocalProxy只是中间的媒介罢了
Request类在 flask的wrappers.py文件中
#目标,寻找request.form的form args cookies这些成员到底在哪
#flask的wrappers.py文件
class Request(RequestBase):
然而很遗憾,我们从flask的Rquest类里没有找到form这个变量名
我们发现它继承了RequestBase,这个RequestBase是werkzeug的wrappers.py文件里的Request类
from werkzeug.wrappers import Request as RequestBase, Response as ResponseBase

于是我们找到werkzeug
```python
#werkzurg wrappers.py
class Request(BaseRequest, AcceptMixin, ETagRequestMixin,
              UserAgentMixin, AuthorizationMixin,
              CommonRequestDescriptorsMixin):
    """Full featured request object implementing the following mixins:

    - :class:`AcceptMixin` for accept header parsing
    - :class:`ETagRequestMixin` for etag and cache control handling
    - :class:`UserAgentMixin` for user agent introspection
    - :class:`AuthorizationMixin` for http auth handling
    - :class:`CommonRequestDescriptorsMixin` for common headers
    """
#这个Request就这么一段,里面没有定义任何方法和变量,
#这里继承的类功能如上面注释所说明
#经过辛苦找寻,我们发现form args等参数就在BaseRequest方法中
#这里列出form的部分
    @cached_property
    def form(self):
        """省略注释"""
        self._load_form_data()
        return self.form
看看
def _load_form_data(self):
        if 'form' in self.__dict__:
            return

        _assert_not_shallow(self)

        if self.want_form_data_parsed:
            content_type = self.environ.get('CONTENT_TYPE', '')
            content_length = get_content_length(self.environ)
            mimetype, options = parse_options_header(content_type)
            parser = self.make_form_data_parser()
            data = parser.parse(self._get_stream_for_parsing(),
                                mimetype, content_length, options)
        else:
            data = (self.stream, self.parameter_storage_class(),
                    self.parameter_storage_class())
                    
        d = self.__dict__
        d['stream'], d['form'], d['files'] = data
```