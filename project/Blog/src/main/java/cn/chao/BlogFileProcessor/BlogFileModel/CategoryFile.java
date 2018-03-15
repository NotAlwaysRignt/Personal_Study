package cn.chao.BlogFileProcessor.BlogFileModel;

import java.io.File;
import java.util.HashMap;

public class CategoryFile {
    int id = -1;
    public String name;
    public HashMap<String,SeriesFile> seriesFiles;
    public File file;

    public CategoryFile(File file) {
        this.seriesFiles = new HashMap<>();
        this.file = file;
        this.name = file.getName();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public HashMap<String, SeriesFile> getSeriesFiles() {
        return seriesFiles;
    }

    public void setSeriesFiles(HashMap<String, SeriesFile> seriesFiles) {
        this.seriesFiles = seriesFiles;
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }
}
