package cn.chao.test.FileProcessor;

import cn.chao.BlogFileProcessor.BlogFileModel.CategoryFile;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.*;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;

public class FileIoTest {
    public String uri = "/static/posts/img/";
    public String url;

    @BeforeEach
    void initurl(){
        url = CategoryFile.class.getResource(uri).getFile();
    }

    @Test
    void readCategoryFiles() {
        url = CategoryFile.class.getResource(uri).getFile();
        System.out.println(uri);
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        File file = new File(url);
        File[] CategoryFiles = file.listFiles();
        for (File category : CategoryFiles) {
            if (category.isDirectory()) {
                System.out.println(category.getName());
                for (File series : category.listFiles()) {
                    if (series.isDirectory()) {
                        System.out.println("---" + series.getName());
                    }
                }
                String datetime = df.format(category.lastModified());
                Timestamp time = new Timestamp(category.lastModified());
                System.out.println(time);
            }

        }
    }


    @Test
    void deleteAllFiles() {
        File file = new File(url + "temp");
        deleteFile(file);

    }

    void deleteFile(File file) {
        try {
            for (File f : file.listFiles()) {
                if (f.isFile()) {
                    f.delete();
                } else {
                    deleteFile(f);
                }
            }
            file.delete();
        } catch (Exception e) {
            System.out.println("fail to delte file"+file.getName());
        }

    }



    @Test
    void MakeDirectories(){
        File f = new File(url+1+"/"+2);
        f.mkdirs();
    }


    @Test
    void readFile(){
        String oldPath = url+"2/"+"dsada";
        File file = new File(oldPath);
        try {
            InputStream fileinputStream = new FileInputStream(file);
            InputStreamReader inputStreamReader = new InputStreamReader(fileinputStream,"UTF-8");
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            StringBuffer sb = new StringBuffer();
            String text = null;
            while((text = bufferedReader.readLine()) != null){
                sb.append(text);
            }
            System.out.println(sb.toString());
        }
        catch(FileNotFoundException e){
            System.out.println(e.getMessage());
        }
        catch (UnsupportedEncodingException e){
            System.out.println(e.getMessage());
        }
        catch (IOException e){
            System.out.println(e.getMessage());
        }

    }



    @Test
    public void copyFile() {
        String oldPath = url+"2/"+"dsada";
        String newPath = url + 1 + "/2"+"/abcd";
        try {
            int bytesum = 0;
            int byteread = 0;
            File oldfile = new File(oldPath);
            if (oldfile.exists() && oldfile.isFile()) { //文件存在时
                InputStream inStream = new FileInputStream(oldPath); //读入原文件
                FileOutputStream fs = new FileOutputStream(newPath);
                byte[] buffer = new byte[1444];
                int length;
                while ( (byteread = inStream.read(buffer)) != -1) {
                    bytesum += byteread; //字节数 文件大小
                    System.out.println(bytesum);
                    fs.write(buffer, 0, byteread);
                }
                inStream.close();
            }
        }
        catch (Exception e) {
            System.out.println("复制单个文件操作出错");
            e.printStackTrace();

        }

    }

    @Test
    public void testFileClassGetPath(){
        File file = new File(url);
        try{
            System.out.println(file.getCanonicalPath());
        }catch(IOException e){
            System.out.println(e.getMessage());
        }
    }

}
