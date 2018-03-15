package cn.chao.dao;

import cn.chao.BlogFileProcessor.BlogFileModel.ArticleFile;
import cn.chao.entity.Article;

import java.util.List;

public interface ArticleMapper {
    /*
    * 通过Series 的 id 获取这个Series 对应的所有文章
    * */
    public List<Article> selectArticle(int series_id);


    //根据id获得文章
    public Article selectArticleById(int article_id);


    //按rank获取这个series下的所有article,number为数量,如果<=0,则会传回所有符合 series_id 的文章
    public List<Article> selectArticleByRankWithoutContent(Integer series_id,Integer number);

    //通过Series 的 id 获取这个Series 对应的所有文章,但不包括内容
    public List<Article> selectLatestArticleWithoutContent(Integer series_id, Integer number);

    public List<Article> selectLatestArticle();

    /*将文件对象存储到数据库中,增加一篇新的文章
    * */
    public int insertArticle(ArticleFile article);

    /*
    * 修改Article的内容及时间(修改时间直接通过sql实现)
    * */
    public int updateArticle(ArticleFile article);

    /*删除Article*/
    public int deleteArticle(int id);
}
