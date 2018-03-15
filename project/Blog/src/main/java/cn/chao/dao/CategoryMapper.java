package cn.chao.dao;
import cn.chao.BlogFileProcessor.BlogFileModel.CategoryFile;
import cn.chao.entity.Category;

import java.util.List;


public interface CategoryMapper {

    /*
    * 获得全部Category
    * */
    public List<Category> selectCategory();



    // 增添 Category ,id已被设为自增
    public int insertCategory(CategoryFile category);


    // 删除 Category
    public int deleteCategory(int id);

}
