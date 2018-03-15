package cn.chao.dto;

import cn.chao.entity.Category;

import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class CategoryDTO {
    Category category;
    List<SeriesDTO> series_list;

    public CategoryDTO(Category category, List<SeriesDTO> series_list) {
        this.category = category;
        this.series_list = series_list;
    }

    public Category getCategory() {
        return category;
    }

    public void setCategory(Category caregory) {
        this.category = caregory;
    }

    public List<SeriesDTO> getSeries_list() {
        return series_list;
    }

    public void setSeries_list(List<SeriesDTO> series_list) {
        this.series_list = series_list;
    }
}
