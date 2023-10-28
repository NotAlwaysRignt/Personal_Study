Tomcat 完成了哪些功能,它和我们自定义的Servlet是什么关系?

Tomcat 是 Java 编写的服务器,因此要运行Tomcat 必须安装 JRE

Tomcat 完成了 Socket 的封装,以及 HTTP解析, TCP 通信的功能是由 Tomcat完成的

Tomcat 实现了对 Web.xml ,server.xml 文件的解析

当客户端发送请求时,Tomcat 通过 web.xml的 &lt;url-pattern&gt; 完成了URL的匹配, 找到相应的 servlet,如果还
未加载,就用 反射机制加载,然后生成相应的 servlet实例,这样就完成了 Tomcat 与我们自定义的servlet的结合
