package cn.chao.test.FileProcessor;

import cn.chao.BlogFileProcessor.DatabaseSynchronizer;
import cn.chao.SpringConfig.BeanConfig;
import org.junit.jupiter.api.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static org.junit.jupiter.api.Assertions.*;

class DatabaseSynchronizerTest {

    @Test
    public void generateTest(){
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        DatabaseSynchronizer synchronizer = (DatabaseSynchronizer) ctx.getBean("databaseSynchronizer");
        System.out.println("Success!");
    }

}