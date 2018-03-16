####  注意Application类的位置!
SpringBoot项目的Bean装配默认规则是根据Application类所在的包位置从上往下扫描！“Application类”是指SpringBoot项目入口类。这个类的位置很关键：如果Application类所在的包为：com.boot.app，则只会扫描com.boot.app包及其所有子包，如果service或dao所在包不在com.boot.app及其子包下，则不会被扫描！即, 把Application类放到dao、service所在包的上级，com.boot.Application知道这一点非常关键
> 引自:   http://blog.csdn.net/huihuilovei/article/details/62041734  

### 依赖注入
根据 Springboot 官方文档,Springboot 在依赖的使用上和 Spring Framework 是一模一样的
 > You are free to use any of the standard Spring Framework techniques to define your beans and their injected dependencies  

**使用注解的方式:**  
> Annotation injection is performed before XML injection, thus the latter configuration will override the former for properties wired through both approaches.  

注解会被先读取,然后才读取XML配置,也就是说,如果一个bean既在注解中被定义,又在XML中被定义,XML中定义的bean会覆盖注解定义的bean  

#### 使用　@bean 和 @Configuration 配置
最新的 Spring 版本允许使用注解的方式进行依赖注入的配置,以达到和用 XML 配置等价的效果.  
> spring官方文档,基于注解进行IOC配置:https://docs.spring.io/spring-framework/docs/current/spring-framework-reference/core.html#beans-java-basic-concepts

@Configuration 用于注释类,@Bean用于注释方法.  
被@Bean注释的方法,多数情况下是@Configuration注释的类的方法,当然有时候也可以是@Component注释的类的方法 

被@Configuration注释的类表明这个类就是用来说明 bean 的各种定义和依赖关系.即这个类是用来描述配置的.  
@Bean 相当于 XML 里面的&lt;bean&gt;标签,每个@Bean修饰的方法都用于描述一个对象(注意是对象不是类,另外方法和对象是一对一的关系),这些对象将被Spring IOC  容器管理  ,官方文档例子:
```java
@Configuration
public class AppConfig {

    @Bean
    public MyService myService() {
        return new MyServiceImpl();
    }
}
```
等价于:
```XML
<beans>
    <bean id="myService" class="com.acme.services.MyServiceImpl"/>
</beans>


```

