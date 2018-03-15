package cn.chao.test.synchronizer;

import cn.chao.BlogFileProcessor.DatabaseSynchronizer;
import cn.chao.SpringConfig.BeanConfig;
import org.junit.jupiter.api.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static org.junit.jupiter.api.Assertions.*;

class DatabaseSynchronizerTest {
    @Test
    void synchronizeFileAndDatabase() {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        DatabaseSynchronizer syn = (DatabaseSynchronizer) ctx.getBean("databaseSynchronizer");
        syn.synchronizeFileAndDatabase();
    }


}