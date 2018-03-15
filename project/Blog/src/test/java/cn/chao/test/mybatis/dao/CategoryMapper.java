package cn.chao.test.mybatis.dao;
import java.util.List;


public interface CategoryMapper {

    /*
    * 获得全部类别的名称
    * */
    List<String> getAllCategorieNames();
}
