package cn.chao.test.springbootTest;

import cn.chao.test.springbootTest.DenpendencyInjection.BeanConfig;
import cn.chao.test.springbootTest.DenpendencyInjection.Person;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        Person p = ctx.getBean(Person.class);   //官网用ApplicationContext接收不了
        p.say();
        SpringApplication.run(Application.class, args);
    }

}
