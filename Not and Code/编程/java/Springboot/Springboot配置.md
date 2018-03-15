####  注意Application类的位置!
SpringBoot项目的Bean装配默认规则是根据Application类所在的包位置从上往下扫描！“Application类”是指SpringBoot项目入口类。这个类的位置很关键：如果Application类所在的包为：com.boot.app，则只会扫描com.boot.app包及其所有子包，如果service或dao所在包不在com.boot.app及其子包下，则不会被扫描！即, 把Application类放到dao、service所在包的上级，com.boot.Application知道这一点非常关键
> 引自:   http://blog.csdn.net/huihuilovei/article/details/62041734  

#### 返回 json 数据    
> Any Spring @RestController in a Spring Boot application should render JSON response by default as long as Jackson2 is on the classpath  

用 @RestController 修饰的类,其所有方法返回的都是json,例子如下:  
```java
@RestController
public class MyController {

    @RequestMapping("/thing")
    public MyThing thing() {
            return new MyThing();
    }

}  
```  

### 默认路径
Springboot 会默认到 resources/templates 目录下寻找 html, 静态文件会默认到 resources/static 目录下找寻,如使用&lt;img src="img/hello.png"&gt;时文件路径为 resources/static/img/hello.png