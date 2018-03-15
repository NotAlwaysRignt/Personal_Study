package cn.chao.test.Junit5;


import org.junit.jupiter.api.Test;

import java.io.IOException;

import static  org.junit.jupiter.api.Assertions.assertEquals;


public class JunitTest {

    @Test
    public void firstJunit5Test(){
        assertEquals(6,7-2);
    }

    @Test
    public void printHelloWorld(){
        System.out.println("Hello World!");
    }


    @Test
    public void testPrintExcept(){
        try{
            thorowExcep();
        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }
    }


    void thorowExcep() throws IOException{
        throw new IOException("I don't know would it be printed");
    }


}
