package cn.chao.test.springbootTest.DenpendencyInjection;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig{

    @Bean
    public Person student(){
        return new Student();
    }
}
