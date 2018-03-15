package cn.chao.test.FileProcessor;

import cn.chao.BlogFileProcessor.BlogFileModel.BlogFileSystem;
import cn.chao.SpringConfig.BeanConfig;
import org.junit.jupiter.api.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;


public class BlogSystemTest {


    @Test
    public void testinitBlogSystem(){
        String url = BlogFileSystem.class.getResource("/static/posts/text").getFile();
        BlogFileSystem blog = new BlogFileSystem(url);
        System.out.println("OK");
    }

    @Test
    public void testBlogFileSystemBean(){
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        BlogFileSystem blogFileSystem = ctx.getBean(BlogFileSystem.class);
        System.out.println(blogFileSystem.getCategories().size());
    }

    @Test
    void testStaticResourcePath(){
    }
}
