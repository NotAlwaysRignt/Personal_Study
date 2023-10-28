#coding:utf-8
先上结论:flask在添加路由时采用的思想并不复杂
就是以 url:视图函数 这样的键值对方式,用字典来保存
self.view_functions[endpoint] = view_func #view_functions是一个{}
view_func是自定义的函数(是一个地址)
endpoint 如果没有特别定义,则默认为 view_func.__name__ ,即函数的名字


flask 中，构建这个路由规则的两种方法
1.通过 Flask的 add_url_rule 方法
2.通过 @app.route()这个装饰器(其实内部调用add_url_rule,因此这两种方法本质上等价)



def route(self, rule, **options):
        def decorator(f):
            endpoint = options.pop('endpoint', None)
            self.add_url_rule(rule, endpoint, f, **options)
            return f
        return decorator
        
route是一个装饰器,从用法我们就可以看出
视图函数作为参数f传入
url 作为rule参数传入,methods = ['GET','POST']作为**options传入
生成 {'methods':['GET','POST']}

看看self.add_url_rul

@setupmethod
def add_url_rule(self, rule, endpoint=None, view_func=None, provide_automatic_options=None, **options):
#view_func参数就是自定义的视图函数 ,我们把endponit看成是一个url对应的逻辑处理函数名称  

    #endpoint_from_view_func作用是返回view_func.__name__,即函数的名字(.helper.py文件中)
    if endpoint is None:#如果endpoint没有定义,那就用view_fuction的名字作为endpoint
        endpoint = _endpoint_from_view_func(view_func)
    
    options['endpoint'] = endpoint
    methods = options.pop('methods', None)#这里是HTTP方法,比如GET
    
    #如果未传入methods参数,默认是GET方法
    if methods is None:
        methods = getattr(view_func, 'methods', None) or ('GET',)
    #(string_types定义在_compat.py文件)string_types在python2.x中是(str,)
    #python3.x为 (str, unicode),正如下面raise的内容,methods必须是tuple或list等可迭代对象
    if isinstance(methods, string_types):#
        raise TypeError('Allowed methods have to be iterables of strings, '
                        'for example: @app.route(..., methods=["POST"])')
    #不管是传methods = ['get'],['Get'],['GET']效果是一样的,都会转换为大写
    #并且会将原来的methods转换为一个set() 原来的methods可能是一个list或tuple
    #我们要知道methods具有去重功能
    methods = set(item.upper() for item in methods)

    #如果view_func中没有'required_methods'(一般我们不会给函数定义这个属性)
    #那么required_methods就是一个基础数据类型set(),set内包含一个空的tuple
    required_methods = set(getattr(view_func, 'required_methods', ()))
    
    #关于provide_automatic_options我们较少使用,可以暂且跳过这下面两个判断
    if provide_automatic_options is None:
        provide_automatic_options = getattr(view_func,
            'provide_automatic_options', None)
    #如果没有在view_func中定义'provide_automatic_options',则它扔为None
    
    if provide_automatic_options is None:
        if 'OPTIONS' not in methods:
            provide_automatic_options = True
            required_methods.add('OPTIONS')required_methods
            #此时required_methods为{'OPTIONS'},是一个set()
        else:
            provide_automatic_options = False

    #上面methods已经被转换成了set()类型,这样就可以作逻辑运算
    methods |= required_methods 
    #假设methods为{'GET'},required_methods经上面 if 'OPTIONS' not in methods 部分
    #处理后为{'OPTIONS'},则运算后methods = {'OPTION', 'GET'}
    
    #url_rule_class = Rule (Rule这个类定义在 werkzeug的routing.py文件中)
    rule = self.url_rule_class(rule, methods=methods, **options)
    rule.provide_automatic_options = provide_automatic_options

    self.url_map.add(rule)
    if view_func is not None:#判断 新添加的视图函数 是否与以有的视图函数重名
        old_func = self.view_functions.get(endpoint)#若重名,将原来的覆盖,并报出警告
        if old_func is not None and old_func != view_func:
            raise AssertionError('View function mapping is overwriting an '
                                 'existing endpoint function: %s' % endpoint)
        self.view_functions[endpoint] = view_func
        
add_url_rule这个函数干的最主要的两件事就是:
更新 self.url_map 和 self.view_functions 两个变量
在Flask中找到这两个变量        
self.url_map = Map() ( Map 这个类定义在 werkzeug的routing.py文件中)
self.view_functions = {}


self.url_map.add(rule),我们用Rule类的实例 构造了一个 Map类的实例
这两个类的定义都在 werkzeug的routing.py文件中
简单演示这两个类的用法:
#节选自routing.py的注释

>>> m = Map([
...     # Static URLs
...     Rule('/', endpoint='static/index'),
...     Rule('/about', endpoint='static/about'),
...     Rule('/help', endpoint='static/help'),
...     ])
... ], default_subdomain='www')

可以匹配url #可以构建url,这里不展开讲
>>> c = m.bind('example.com')
>>> c.match("/")
('static/index', {})

上面是werkzeug路由的方法,用Map()类封装了字典的功能,这样就可以用键值对的形式才访问
url对应的视图函数


到这里我们看出,其实flask在添加路由时采用的思想并不复杂
就是以 url:视图函数 这样的键值对方式,用字典来保存

