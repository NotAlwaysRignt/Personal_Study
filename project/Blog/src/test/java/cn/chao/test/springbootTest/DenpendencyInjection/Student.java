package cn.chao.test.springbootTest.DenpendencyInjection;

public class Student implements Person{
    public int number;

    @Override
    public void say(){
        System.out.println("Hello my Teacher");
    }
}
