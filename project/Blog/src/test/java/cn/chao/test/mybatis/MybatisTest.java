package cn.chao.test.mybatis;

import cn.chao.BlogFileProcessor.BlogFileModel.CategoryFile;
import cn.chao.dao.ArticleMapper;
import cn.chao.dao.CategoryMapper;
import cn.chao.dao.ImageMapper;
import cn.chao.entity.Article;
import cn.chao.entity.Category;
import cn.chao.entity.Image;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test; // 注意Test标签别引用错误了!
import org.junit.jupiter.api.BeforeEach;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

public class MybatisTest {
    public static String url;
    private static String resource = "mybatis-config.xml";
    private static InputStream resourceInputStream;
    private static SqlSessionFactory sqlSessionFactory;
    private SqlSession sqlSession;

    @BeforeAll
    public static void init(){
        url = CategoryFile.class.getResource("/static/posts/text/").getFile();


        try {
            resourceInputStream = Resources.getResourceAsStream(resource);
        } catch (IOException e) {
            System.out.println(e.getMessage());;
        }

        sqlSessionFactory = new SqlSessionFactoryBuilder().build(resourceInputStream);
    }

    @BeforeEach
    public void initSession() {
        sqlSession = sqlSessionFactory.openSession();
    }


    @AfterEach
    void closesession(){
        sqlSession.close();
    }

    @Test
    public void insertCategoryTest() {
        CategoryMapper mapper = sqlSession.getMapper(CategoryMapper.class);
        CategoryFile category = new CategoryFile(new File(url+"编程"));
        category.setId(2);
        try {
            System.out.println(mapper.insertCategory(category));
            System.out.println("新生成的主键:" + category.getId());
        } catch (Exception e) {
            e.printStackTrace();
        }
        //sqlSession.commit();

    }

    @Test
    public void deleteCategoryTest(){
        CategoryMapper mapper = sqlSession.getMapper(CategoryMapper.class);
        try {
            int temp = mapper.deleteCategory(8);
            System.out.println("temp:   "+temp);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }


    @Test
    public void selectCategoryTest(){
        CategoryMapper mapper = sqlSession.getMapper(CategoryMapper.class);
        try{
            List<Category> categories = mapper.selectCategory();
            sqlSession.commit();
            for(Category categorie:categories){
                System.out.println("--------------------------------");
                System.out.println(categorie.getName());
                System.out.println(categorie.getId());
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    @Test
    void selectImage(){
        ImageMapper imageMapper = sqlSession.getMapper(ImageMapper.class);
        List<Image> images = imageMapper.selectImage(-1,1);
    }

    @Test
    void selectRecentArticle(){
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);
        List<Article> articles = articleMapper.selectLatestArticle();
        System.out.println("ok");
    }

    @Test
    void selectArticleWithoutContent(){
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);
        List<Article> articles = articleMapper.selectLatestArticleWithoutContent(117,5);
    }


    @Test
    void selectArticleByRankWithoutContent(){
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);
        List<Article> articles = articleMapper.selectArticleByRankWithoutContent(117,null);

    }

    @Test
    void selectArticleById(){
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);
        Article article = articleMapper.selectArticleById(159);

    }
}
