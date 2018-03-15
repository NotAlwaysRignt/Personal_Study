1.服务器运行
2.用户请求到来,服务器会检查相应Servlet实例是否存在,若不存在,装载Servlet类创建一个Servlet实例

3.调用init(ServletConfig)方法
4.调用service方法 (Servlet Request Servlet Response)

之后如果有同样的请求到来,在相应Servlet实例被创建后则直接执行 3 和 4

5.服务器关闭时,调用 Servlet 的 Destroy() 方法,销毁实例


关于Servlet类(我们自定义编写的)加载时期的补充:

1.如果在web.xml没有特别说明,那么Servlet类并不是在Tomcat服务器启动时被加载的,而是在用户第一次访问URL时,
这个URL所对应的Servlet类才会调用其构造函数被实例化,Servlet类被实例时最先调用构造函数,接着会调用 init()
方法.一旦被实例化后便会一直存在于内存中,直到服务器关闭才被销毁.

2.在web.xml中的<Servlet></Servlet>标签内添加<loadon-startup></loadon-startup>标签,那么这个Servlet类就
会在服务器启动的时候就被加载而不是用户第一次访问时实例化,<loadon-startup></loadon-startup>标签内数字越小
优先级越高,越先开始加载,比如标记了 <loadon-startup> 1 </loadon-startup> 的类,
比标记了<loadon-startup> 2 </loadon-startup>类要先开始加载.

3.用Tomcat启动工程,在源代码被修改后,Tomcat会自动重新编译代码,并重新实例化 之前已经启动的Servlet实例