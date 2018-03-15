package cn.chao.BlogFileProcessor.BlogFileModel;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;



/**
 * 博客文件系统,这个类在初始化后可以将整个博客文件的架构读取下来,文件必须按固定的目录结构配置
 * 1. Category 分类,如"历史","编程"
 * 2. Series,位于每个 Category 目录下,表示某一系列的文章,即使只有一篇文章也需要Series,如"Javascript教程"
 *           每个Series 目录下还有一个 "img"(名字必须为"img") 的目录,存储所有这一系列文章的图片
 * 3. ArticleDTO , 位于每个Series目录下,每个markdown文件对应一个 ArticleDTO 对象,如 "JavaScript基本类型"
 * */
public class BlogFileSystem {
    public String url;
    public HashMap<String,CategoryFile> categories;

    public BlogFileSystem(String url){
        this.categories = new HashMap<>();
        this.url = url;
        try {
            initCategories(url);  // 从顶层开始读取整个文件夹
        }catch (FileNotFoundException e){
            System.out.println(e.getMessage());
        }
    }

    private void initCategories(String url) throws FileNotFoundException{
        File file = new File(url);
        if(!file.exists()){
            throw new FileNotFoundException();
        }
        for(File f:file.listFiles()){
            if(f.isDirectory()) {
                CategoryFile category = new CategoryFile(f);
                categories.put(f.getName(), category);
                initSeries(category);
            }
        }
    }

    private void initSeries(CategoryFile categoryFile){
        for(File f:categoryFile.getFile().listFiles()){
            if(f.isDirectory()){
                SeriesFile series = new SeriesFile(f);
                categoryFile.getSeriesFiles().put(f.getName(),series);
                initArticle(series);
            }
        }
    }



    private void initArticle(SeriesFile seriesFile){
        for(File f:seriesFile.getFile().listFiles()){
            if(f.isFile()){
                ArticleFile articleFile = new ArticleFile(f);
                seriesFile.getArticleFiles().put(f.getName().split(".md")[0],articleFile);
            }
            else if(f.isDirectory() && f.getName().equals("img")){
                for(File imageFile:f.listFiles()) {
                    if(imageFile.isFile()) {
                        seriesFile.getImageFiles().add(imageFile);
                    }
                }
            }
        }
    }


    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public HashMap<String, CategoryFile> getCategories() {
        return categories;
    }

    public void setCategories(HashMap<String, CategoryFile> categories) {
        this.categories = categories;
    }
}
