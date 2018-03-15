我们在自定义编写一个Servlet类时，会继承一个 HttpServlet 类，并重写它的 doGet 和 doPost方法
HttpServlet 是一个抽象类，它实现了HTTP协议， 其父类是GenericServlet
GenericServlet类是一个抽象类，它是与协议无关的Servlet，它继承了Servlet接口
Servlet接口有三个方法，Init(), service(), destroy()

