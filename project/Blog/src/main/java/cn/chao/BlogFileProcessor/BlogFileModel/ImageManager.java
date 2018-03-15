package cn.chao.BlogFileProcessor.BlogFileModel;

import cn.chao.entity.Image;
import org.springframework.beans.factory.annotation.Autowired;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.List;


public class ImageManager {
    private List<Image> imageToDelete; //这里的Image指向要删除的图片文件
    private List<File> imageToCopy; //要复制到img文件夹的图片

    public File targetImgFile; //img文件夹,存放所有文章用到的图片

    public ImageManager(File targetImgFile) {
        this.targetImgFile = targetImgFile;
        this.imageToCopy = new LinkedList<File>();
        this.imageToDelete = new LinkedList<Image>();
    }

    public String addImageFileToCopy(File imageFile){
        imageToCopy.add(imageFile);
        return targetImgFile.getAbsolutePath()+"/"+imageFile.getName();

    }

    public void addImageFileToDelete(List<Image> images){
        imageToDelete.addAll(images);
    }
    public void deleteImageFiles(){
        for(Image i:imageToDelete){
            File f = new File(i.getPath());
            try{
                if(f.exists() && f.isFile()){
                    f.delete();
                }
            } catch (Exception e){
                System.out.println("图片删除出错: "+ e.getMessage());
            }
        }

    }

    public void insertImageFiles(){
        for(File f:imageToCopy) {
            if (f.exists() && f.isFile()) {
                copyFile(f);
            }
        }
    }


    public void copyFile(File src) {
        String oldPath = src.getAbsolutePath();
        String newPath = targetImgFile.getAbsolutePath()+"/"+src.getName();
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
            System.out.println("复制图片出错: " + oldPath);
        }

    }

    public void setTargetImgFile(File targetImgFile) {
        this.targetImgFile = targetImgFile;
    }

    public List<Image> getImageToDelete() {
        return imageToDelete;
    }

    public void setImageToDelete(List<Image> imageToDelete) {
        this.imageToDelete = imageToDelete;
    }

    public List<File> getImageToCopy() {
        return imageToCopy;
    }

    public void setImageToCopy(List<File> imageToCopy) {
        this.imageToCopy = imageToCopy;
    }
}
