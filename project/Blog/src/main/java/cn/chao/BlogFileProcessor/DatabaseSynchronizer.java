package cn.chao.BlogFileProcessor;

import cn.chao.BlogFileProcessor.BlogFileModel.*;
import cn.chao.dao.ArticleMapper;
import cn.chao.dao.CategoryMapper;
import cn.chao.dao.ImageMapper;
import cn.chao.dao.SeriesMapper;
import cn.chao.entity.Article;
import cn.chao.entity.Category;
import cn.chao.entity.Image;
import cn.chao.entity.Series;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import java.io.File;
import java.io.IOException;
import java.util.*;

public class DatabaseSynchronizer implements ISynchronizer{
    private BlogFileSystem blogFileSystem;
    private ImageManager imageManager;  //管理图像文件
    private SqlSessionFactory sqlSessionFactory;
    private SqlSession sqlSession;


    //数据库映射对象
    private ImageMapper imageMapper;
    private CategoryMapper categoryMapper;
    private SeriesMapper seriesMapper;
    private ArticleMapper articleMapper;

    @Override
    public void synchronizeFileAndDatabase(){
        sqlSession = sqlSessionFactory.openSession();
        imageMapper = sqlSession.getMapper(ImageMapper.class);
        categoryMapper = sqlSession.getMapper(CategoryMapper.class);
        seriesMapper = sqlSession.getMapper(SeriesMapper.class);
        articleMapper = sqlSession.getMapper(ArticleMapper.class);


        try {
            checkCategory();
            updateCategoryInDatabase();
            sqlSession.commit();

            imageManager.deleteImageFiles();
            imageManager.insertImageFiles();
            System.out.println("数据库数据和文件夹数据已同步!");
        } catch(Exception e){
            System.out.println("同步数据库与文件系统失败: "+ e.getMessage());
        }
        finally {
            sqlSession.close();
        }
    }

    /*数据库中的数据最终应该与文件系统中的数据一致
      先将数据库中的数据与文件系统中的数据一一对比
      1.数据库中存在,而文件系统中存在的数据,应该从数据库中删除
      2.数据库中和文件系统中保持一致的数据,不需要做任何处理,因此从 BlogFileSystem 对象中去掉
      3.比对完毕后,BlogFileSystem对象中的所有数据都要被更新到数据库中(update或insert)
      对图片的管理
      会把所有的图片都重新拷贝一份到/posts/img目录下,以便网站访问
      如果数据库中的Category 或 Series与文件系统中的不一致,就把/posts/img 下相应的图片全部删除
      (Image表中的path图片的位置)从文件系统中重新将图片拷贝过去,并添加记录到Image表中(每张图片对应一行)
     */

    private void checkCategory() {
        List<Category> categories = categoryMapper.selectCategory();
        HashMap<String, CategoryFile> categoryFiles = blogFileSystem.getCategories();
        for (Category c : categories) {
            String name = c.getName();
            if (categoryFiles.containsKey(name)) {//如果文件系统中和数据库中均存在这个Category文件,检查Series是否一致
                boolean category_changed = false;
                categoryFiles.get(name).setId(c.getId());
                category_changed = checkSeries(c, categoryFiles.get(name));
                if (!category_changed) { //将数据库中信息和文件系统一致的部分从BlogFileSystem里删除
                    categoryFiles.remove(name);
                }
            } else {
                List<Image> images = imageMapper.selectImage(c.getId(), -1);
                categoryMapper.deleteCategory(c.getId());
                imageManager.addImageFileToDelete(images);

            }
        }
    }

    private boolean checkSeries(Category categoryModel, CategoryFile categoryFile) {
        boolean series_changed = false; //每一此循环时都要重置false
        List<Series> series = seriesMapper.selectSeries(categoryModel.getId());
        HashMap<String, SeriesFile> seriesFile = categoryFile.getSeriesFiles();
        List<Series> seriesToDelete = new LinkedList<Series>();

        int unchanged_file = seriesFile.size();
        for (Series s : series) {
            unchanged_file--;
            String name = s.getName();
            if (seriesFile.containsKey(name)) {//如果文件系统中和数据库中均存在这个Series的信息,检查Article是否一致
                boolean article_changed = false;
                seriesFile.get(name).setId(s.getId()); //用于此后重新添加Image信息
                seriesFile.get(name).setCategory_id(s.getCategory_id());
                article_changed = checkArticle(s, seriesFile.get(name));
                if (article_changed) {
                    List<Image> images = imageMapper.selectImage(-1, s.getId());
                    imageManager.addImageFileToDelete(images);
                    for(Image image:images){
                        imageMapper.deleteImage(image.getId());
                    }
                    series_changed = true;
                } else {
                    seriesFile.remove(s.getName());
                }
            } else {//删除这个Series的所有内容,包括图片
                List<Image> images = imageMapper.selectImage(-1, s.getId());
                imageManager.addImageFileToDelete(images);
                seriesToDelete.add(s);
            }
        }
        if (unchanged_file!=0) series_changed = true;

        //删除已经不需要的备份文件
        for(Series s:seriesToDelete) {
            seriesMapper.deleteSeries(s.getId()); //包括删除指向这个Series的图片
        }

        return series_changed;
    }

    private boolean checkArticle(Series seriesModel, SeriesFile seriesFile) {
        boolean article_changed = false; //只要有一个Article被修改,说明这个Series就被修改了
        List<Article> article = articleMapper.selectArticle(seriesModel.getId());
        HashMap<String, ArticleFile> articleFile = seriesFile.getArticleFiles();
        List<Article> articleTodelete = new LinkedList<Article>();

        int unchanged_file = articleFile.size();
        for (Article a : article) {
            unchanged_file--;
            String title = a.getTitle();
            if (articleFile.containsKey(title)) {
                articleFile.get(title).setCategory_id(a.getCategory_id());
                articleFile.get(title).setSeries_id(a.getSeries_id());
                if (a.getTimestamp() == articleFile.get(title).getTimestamp()) {
                    articleFile.remove(title);
                } else {
                    //比较数据库和文件的timestamp,如果不一致,说明文件被修改过,需要更新
                    article_changed = true;
                    articleFile.get(title).setId(a.getId());
                }
            } else {
                articleTodelete.add(a);
            }
        }
        for(Article a:articleTodelete){
            articleMapper.deleteArticle(a.getId());
        }

        if (unchanged_file!=0) article_changed = true;
        return article_changed;
    }

    private void updateCategoryInDatabase() {
        int result = 0;
        for (Map.Entry<String, CategoryFile> categoryFileEntry : this.blogFileSystem.getCategories().entrySet()) {
            CategoryFile categoryFile = categoryFileEntry.getValue();
            if (categoryFile.getId() == -1) {
                result = categoryMapper.insertCategory(categoryFile);
                if(result!=0){
                    updateSeriesInDatabase(categoryFile);
                }
            }
            else{//只有更新数据库的Category成功才可以更新Series
                updateSeriesInDatabase(categoryFile);
            }
        }

    }

    private void updateSeriesInDatabase(CategoryFile categoryFile) {
        int result;
        int category_id = categoryFile.getId();
        for (Map.Entry<String, SeriesFile> seriesFileEntry : categoryFile.getSeriesFiles().entrySet()) {
            SeriesFile seriesFile = seriesFileEntry.getValue();
            seriesFile.setCategory_id(category_id);

            for(File imageToCopy:seriesFile.getImageFiles()){
                imageManager.addImageFileToCopy(imageToCopy);
            }
            if(seriesFile.getId()==-1) {
                result = seriesMapper.insertSeries(seriesFile);
                if(result!=0) {
                    updateArticleInDatabase(seriesFile);
                }
            }else {
                updateArticleInDatabase(seriesFile);
            }

            for(File image:seriesFile.getImageFiles()){
                String imageCopiedPath = imageManager.addImageFileToCopy(image);
                imageMapper.insertImage(imageCopiedPath,seriesFile.getCategory_id(), seriesFile.getId());
            }

        }

    }

    private void updateArticleInDatabase(SeriesFile seriesFile){
        int series_id = seriesFile.getId();
        int category_id = seriesFile.getCategory_id();
        for(Map.Entry<String,ArticleFile> articleFileEntry:seriesFile.getArticleFiles().entrySet()){
            ArticleFile articleFile = articleFileEntry.getValue();
            articleFile.setSeries_id(series_id);
            articleFile.setCategory_id(category_id);

            if(articleFile.getId()==-1){
                articleMapper.insertArticle(articleFile);
            }
            else{
                articleMapper.updateArticle(articleFile);
            }

        }
    }

    @Autowired
    public void setSqlSessionFactory(@Qualifier("sqlSessionFactory") SqlSessionFactory sqlSessionFactory) {
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Autowired
    public void setBlogFileSystem(@Qualifier("blogFileSystem") BlogFileSystem blogFileSystem) {
        this.blogFileSystem = blogFileSystem;
    }


    @Autowired
    public void setImageManeger(@Qualifier("imageFileManager") ImageManager imageManeger) {
        this.imageManager = imageManeger;
    }
}
