package cn.chao;

import cn.chao.BlogFileProcessor.DatabaseSynchronizer;
import cn.chao.SpringConfig.BeanConfig;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        init();
        SpringApplication.run(Application.class, args);
    }

    private static void init(){
        /*synchronize File and Database*/
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        DatabaseSynchronizer databaseSynchronizer =  (DatabaseSynchronizer) ctx.getBean("databaseSynchronizer");
        databaseSynchronizer.synchronizeFileAndDatabase();


    }


}