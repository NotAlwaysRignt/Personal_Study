package cn.chao.dao;

import cn.chao.entity.Image;

import java.util.List;

public interface ImageMapper {
    public List<Image> selectImage(int category_id, int series_id);
    public int insertImage(String path,int category_id, int series_id);
    public int deleteImage(int id);
}
