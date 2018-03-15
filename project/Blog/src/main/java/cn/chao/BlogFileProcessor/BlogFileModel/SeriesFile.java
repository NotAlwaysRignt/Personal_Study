package cn.chao.BlogFileProcessor.BlogFileModel;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class SeriesFile {
    public int id = -1;
    public int category_id = -1;
    public String name;
    public String description;
    int rank;
    public File file;
    public List<File> ImageFiles;
    public HashMap<String,ArticleFile> articleFiles;

    public SeriesFile(File file) {
        this.articleFiles = new HashMap<>();
        this.ImageFiles = new ArrayList<>();
        this.file = file;
        this.name = file.getName();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCategory_id() {
        return category_id;
    }

    public void setCategory_id(int category_id) {
        this.category_id = category_id;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }

    public HashMap<String, ArticleFile> getArticleFiles() {
        return articleFiles;
    }

    public void setArticleFiles(HashMap<String, ArticleFile> articleFiles) {
        this.articleFiles = articleFiles;
    }

    public List<File> getImageFiles() {
        return ImageFiles;
    }

    public void setImageFiles(List<File> imageFiles) {
        ImageFiles = imageFiles;
    }

    public void setImageFiles(File imgFile){
        for(File f:imgFile.listFiles()){
            if(f.isFile()){
                getImageFiles().add(f);
            }
        }
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }
}
