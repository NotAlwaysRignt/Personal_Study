如果 mysql 的 XML 配置文件要从外部的 .properties 文件引入变量,那么应该在&lt;configuration&gt;标签下添加配置:
```xml  
    <properties resource="application.properties"></properties>
```

之后我们可以用占位符替换
比如 application.properties 中有:
username=XiaoMing  
则 XML 中可以用 &lt;property name="XXX" value="${username}"&gt;来引用 XiaoMing 这个变量  

其中 application.properties 是放置 properties 文件的名字,在Springboot项目中,如果 application.properties 是放置在 resources 目录下,则可以这样引用,否则要补全路径  

#### 注意事项
字符串不需要引入 " 符号,在 .properties 文件中
username=XiaoMing 和 username="XiaoMing"  
是不一样的,后者在用 ${username}替换后,引号"也会被引入  

特殊符号不需要转义  
url = jdbc:mysql://localhost:3306/Blog?useSSL=false&characterEncoding=utf-8  
上面的设置中, & 不需要被转义字符&ampt\;
