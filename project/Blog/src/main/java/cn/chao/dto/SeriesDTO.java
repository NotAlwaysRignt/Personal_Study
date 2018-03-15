package cn.chao.dto;

import cn.chao.entity.Article;
import cn.chao.entity.Series;

import java.util.List;

public class SeriesDTO {
    public Series series;
    public List<Article> article_list;

    public SeriesDTO(Series series, List<Article> arcticle_list) {
        this.series = series;
        this.article_list = arcticle_list;
    }

    public Series getSeries() {
        return series;
    }

    public void setSeries(Series series) {
        this.series = series;
    }

    public List<Article> getArticle_list() {
        return article_list;
    }

    public void setArticle_list(List<Article> article_list) {
        this.article_list = article_list;
    }
}
