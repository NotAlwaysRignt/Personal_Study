package cn.chao.dao;

import cn.chao.BlogFileProcessor.BlogFileModel.SeriesFile;
import cn.chao.entity.Series;

import java.util.List;

public interface SeriesMapper {
    public List<Series> selectSeries(int category_id);

    //插入一个Series,主键应该设为自增
    public int insertSeries(SeriesFile series);

    //删除Series,以及以这个Series主键为外键的Image 和 Article
    public int deleteSeries(int id);

}
