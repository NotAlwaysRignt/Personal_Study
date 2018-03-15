package cn.chao.SpringConfig;


import cn.chao.BlogFileProcessor.BlogFileModel.BlogFileSystem;
import cn.chao.BlogFileProcessor.BlogFileModel.ImageManager;
import cn.chao.BlogFileProcessor.DatabaseSynchronizer;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Scope;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

@Configuration
public class BeanConfig {
    //存放着博客文件的位置 和 临时图片存储文件夹的位置
    static String blog_file_url="/home/thinker/code/java/Blog/static/posts";
    //博客里的临时文件会被搬移到这个文件夹里,网页里还可以访问它们
    static String blog_image_url="/home/thinker/code/java/Blog/static/img";

    @Bean
    @Scope("prototype")
    public BlogFileSystem blogFileSystem(){
        return new BlogFileSystem(blog_file_url);
    }


    @Bean
    @Scope("singleton")
    public SqlSessionFactory sqlSessionFactory(){
        String resource = "mybatis-config.xml";
        InputStream resourceInputStream = null;
        try{
            resourceInputStream = Resources.getResourceAsStream(resource);;

        }catch (IOException e){
            System.out.println("初始化数据库失败");
            System.out.println(e.getMessage());
            return null;
        }
        return new SqlSessionFactoryBuilder().build(resourceInputStream);
    }

    @Bean
    @Scope("prototype")
    public DatabaseSynchronizer databaseSynchronizer(){
        DatabaseSynchronizer dbSynchronizer = new DatabaseSynchronizer();
        return dbSynchronizer;
    }


    @Bean
    @Scope("singleton")
    public ImageManager imageFileManager(){
        File targetImageFile = new File(blog_image_url);
        if (!targetImageFile.exists()) {
            System.out.println("文件夹创建于: " + targetImageFile.getAbsolutePath());
            targetImageFile.mkdirs();
        }
        ImageManager imageManeger = new ImageManager(targetImageFile);
        return imageManeger;
    }
}

