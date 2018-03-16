    Tomcat如何运行我们自己编写的Servlet ? 最核心的技术: 反射
    
    Tomcat在启动后会对 web.xml 进行解析, 当客户端发来请求时(Tomcat实现了Socket的封装), 先解析客户请求
中的URL(Tomcat实现了HTTP协议的解析),再与Web.xml 中的<url-pattern> 标签内容作比对,找到这个 URL 所对
应的 <servlet-class>或 <jsp-file> , 接着Tomcat就确定了需要调用的类,如果这个Servlet类还没有被实例
化,那么就用反射函数 Class.forName 去加载我们自己编写的Servlet ,然后再用 newInstance()方法去实例化
这个类, 到此Tomcat就成功地把我们自定义的 Servlet类 和 Tomcat 结合到了一起
