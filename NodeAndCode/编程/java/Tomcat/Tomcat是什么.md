Tomcat 是 Web服务器，和 Nginx / Apache 是并列的概念，和 Nginx，Apache不同的是，Tomcat可以处理
动态的资源(比如HTML里面有动态的用户名)，而 Nginx和Apache 只能返回静态的资源(比如图片，静态的HTML，
CSS等)。 Nginx和Apache本身不支持向客户端返回动态资源，但是他们可以通过其它模块支持，比如 python、PHP
等脚本程序动态生成内容，再交给服务器返回，例子： flask + wsgi + nginx

Tomcat可以看作针对Java语言的服务器，它必须运行在JVM之上，而Python，PHP是不能在Tomcat上运行的，
另外实际应用中 Tomcat 更多地只是作为Servlet的容器， Servlet 才是真正提供 HTTP服务的，Servlet运行Tomcat之上

Tomcat 和 Nginx可以共同使用：
1. 动静态资源分离运用Nginx的反向代理功能分发请求：所有动态资源的请求交给Tomcat，而静态资源的请求（例
如图片、视频、CSS、JavaScript文件等）则直接由Nginx返回到浏览器，这样能大大减轻Tomcat的压力。
2. 负载均衡，当业务压力增大时，可能一个Tomcat的实例不足以处理，那么这时可以启动多个Tomcat实例进行水平
扩展，而Nginx的负载均衡功能可以把请求通过算法分发到各个不同的实例进行处理


事实上，绝大多数编程语言自身就实现了HTTP服务器，比如Python 的SimpleHTTPServer类，只不过这些类比起
专业的Nginx / Apache显得就不够强大，所以通常还是采用Nginx / Apache这些专门的服务器。
