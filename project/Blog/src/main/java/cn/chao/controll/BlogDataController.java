package cn.chao.controll;

import cn.chao.BlogFileProcessor.DatabaseSynchronizer;
import cn.chao.SpringConfig.BeanConfig;
import cn.chao.dao.ArticleMapper;
import cn.chao.dao.CategoryMapper;
import cn.chao.dao.SeriesMapper;
import cn.chao.dto.CategoryDTO;
import cn.chao.dto.Menu;
import cn.chao.dto.SeriesDTO;
import cn.chao.entity.Article;
import cn.chao.entity.Category;
import cn.chao.entity.Series;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.LinkedList;
import java.util.List;

@RestController
public class BlogDataController {

    private SqlSessionFactory sqlSessionFactory;
    public BlogDataController(){
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        this.sqlSessionFactory = (SqlSessionFactory)ctx.getBean("sqlSessionFactory");
    }




    @RequestMapping(value="/menu",method=RequestMethod.GET)
    public Menu getIndexMenu(){
        SqlSession sqlSession = this.sqlSessionFactory.openSession();
        Menu menu = null;
        try {
            menu = newinstanceMenu(sqlSession);
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
        finally {
            sqlSession.close();
        }
        return menu;
    }

    @RequestMapping(value="/article",method=RequestMethod.GET)
    public Article getArticle(@RequestParam(value="article_id")int article_id){
        SqlSession sqlSession = this.sqlSessionFactory.openSession();
        Article article = null;
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);
        try {
            article = articleMapper.selectArticleById(article_id);
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
        finally {
            sqlSession.close();
        }
        return article;
    }


    @RequestMapping(value="/update",method=RequestMethod.GET)
    public String synchronizeDatabaseAndFileSystem(){
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(BeanConfig.class);
        DatabaseSynchronizer synchronizer = ctx.getBean(DatabaseSynchronizer.class);   //官网用ApplicationContext接收不了
        try{
            synchronizer.synchronizeFileAndDatabase();
            return "<h2>同步成功!</h2>";
        }catch (Exception e){
            return "<h2 style=\"text-align:center\">同步失败:"+e.getMessage()+"<h2>";
        }
    }


    private Menu newinstanceMenu(SqlSession sqlSession){
        CategoryMapper categoryMapper = sqlSession.getMapper(CategoryMapper.class);
        SeriesMapper seriesMapper = sqlSession.getMapper(SeriesMapper.class);
        ArticleMapper articleMapper = sqlSession.getMapper(ArticleMapper.class);

        //Initialize Menu
        List<CategoryDTO> categories_list = new LinkedList<>();
        List<Article> latest_articles = articleMapper.selectLatestArticleWithoutContent(null,10);
        Menu menu = new Menu(latest_articles,categories_list);

        //Initialize Menu's member:categoryDTO List
        List<Category> categories = categoryMapper.selectCategory();
        for(Category c:categories){
            //Initialize each CategoryDTO
            CategoryDTO categoryDTO = new CategoryDTO(c,new LinkedList<>());

            List<SeriesDTO> series_list = categoryDTO.getSeries_list();
            List<Series> series = seriesMapper.selectSeries(c.getId());

            //Initialize each SeriesDTO of a CategoryDTO
            for(Series s:series){
                SeriesDTO seriesDTO = new SeriesDTO(s,new LinkedList<>());
                //Initialize each article_list of a SeriesDTO
                seriesDTO.setArticle_list(articleMapper.selectArticleByRankWithoutContent(s.getId(),null));
                series_list.add(seriesDTO);
            }
            menu.getCategory_list().add(categoryDTO);
        }

        return menu;
    }
}
