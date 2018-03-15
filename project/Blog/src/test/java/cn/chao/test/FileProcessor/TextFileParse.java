package cn.chao.test.FileProcessor;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class TextFileParse {
    public String uri = "/static/posts/img/";
    public String url;

    @BeforeEach
    void initurl(){
        url = TextFileParse.class.getResource(uri).getFile();
    }

    @Test
    void split(){
        System.out.println("Hello 世界".split(".md")[0]);
    }

    @Test
    void replaceStr(){

    }



}
