package cn.chao.BlogFileProcessor.BlogFileModel;

import java.io.*;
import java.sql.Timestamp;

public class ArticleFile {
    public File file;
    public int category_id = -1;
    public int series_id = -1;
    public int id = -1;
    public String title;
    public long timestamp;
    public Timestamp datetime;
    public String content;
    public int rank;

    public ArticleFile(File file) {
        this.file = file;
        this.title = file.getName().split(".md")[0]; //去除 md 后缀
        this.timestamp = file.lastModified();
        this.datetime = new Timestamp(this.timestamp);
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }

    public int getCategory_id() {
        return category_id;
    }

    public void setCategory_id(int category_id) {
        this.category_id = category_id;
    }

    public int getSeries_id() {
        return series_id;
    }

    public void setSeries_id(int series_id) {
        this.series_id = series_id;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public Timestamp getDatetime() {
        return datetime;
    }

    public void setDatetime(Timestamp datetime) {
        this.datetime = datetime;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public String getContent() {
        content = readFile(this.file);
        if(content.length()>19000){
            content = "文章长度过长,无法录入";
        }
        return content;
    }

    public void setContent(String content) {

        this.content = content;
    }

    String readFile(File file) {
        String content = null;
        try {
            InputStream fileinputStream = new FileInputStream(file);
            InputStreamReader inputStreamReader = new InputStreamReader(fileinputStream, "UTF-8");
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            StringBuffer sb = new StringBuffer();
            String text = null;
            while ((text = bufferedReader.readLine()) != null) {
                sb.append(text+"\n");
            }
            content = sb.toString();
        } catch (FileNotFoundException e) {
            System.out.println(e.getMessage());
        } catch (UnsupportedEncodingException e) {
            System.out.println(e.getMessage());
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return content;
    }
}