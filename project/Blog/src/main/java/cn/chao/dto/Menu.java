package cn.chao.dto;

import cn.chao.entity.Article;

import java.util.List;

public class Menu {
    public List<Article> latest_articles;
    public List<CategoryDTO> category_list;

    

    public List<Article> getLatest_articles() {
        return latest_articles;
    }

    public void setLatest_articles(List<Article> latest_articles) {
        this.latest_articles = latest_articles;
    }

    public List<CategoryDTO> getCategory_list() {
        return category_list;
    }

    public void setCategory_list(List<CategoryDTO> category_list) {
        this.category_list = category_list;
    }

    public Menu(List<Article> latest_articles, List<CategoryDTO> category_list) {
        this.latest_articles = latest_articles;
        this.category_list = category_list;
    }
}
