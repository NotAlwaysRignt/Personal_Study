Cglib 是使用了扩展的 ASM 框架,即采用了动态字节码生成技术(动态生成 .class文件)来实现基于继承的面向
切面编程(AOP)实现

一般情况下,Cglib 会比 JDK 动态代理速度更快, 
JDK 动态代理 和 Cglib 代理 都会构建新的代理类(创建新的代理类的.class文件), 但 JDK 动态代理类内部并没有
要拦截的函数的具体内容,每次要调用被拦截的函数,还要通过反射去获取,这就带来了性能的损失

而 Cglib 借助 ASM 框架,采用 字节码动态修改技术 ,也就是说,最后 Cglib 最终生成一个基于继承的代理类,
被拦截的函数和我们织入的函数的内容都在这个代理类中,从.class文件的角度说,被拦截的函数和织入的函数的具体内
容也会被写入这个代理类的.class文件中,如此一来,这个代理子类就不用再调用反射去获取被拦截的函数了


Cglib 核心是使用了 ASM 框架,我们来看看这个 ASM 框架干了什么,会反射更快

//自定义一个 User 类
```java
public class User {  
    private int id;  
    private String name;  

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
}  


//看一下ASM 框架下怎么使用这个 User 类
import com.esotericsoftware.reflectasm.MethodAccess;

public class ReflectAsmTest {  
    public static void main(String[] args) {  
        User user = new User();  
        //使用reflectasm生产User访问类  
        MethodAccess access = MethodAccess.get(User.class);  
        //invoke setName方法name值  
        access.invoke(user, "setName", "张三");  
        //invoke getName方法 获得值  
        String name = (String)access.invoke(user, "getName", null);  
        System.out.println(name);  
    }  
}  
```
结果:
张三

上述代码的核心是 MethodAccess.get(User.class) 函数,它返回一个 UserMethodAccess 类,
也就是说它自己构建生成了一个 UserMethodAccess 处理类(生成 UserMethodAccess.class 文件)
它构建的类主要实现了 invoke 方法,这个 invoke 方法内部实现了 User 类内部的所有函数
(逻辑上相当于继承了 User),并通过 switch 来选择调用它的方法

反编译 UserMethodAccess.class 文件:
```java
package com.johhny.ra;  
  
import com.esotericsoftware.reflectasm.MethodAccess;  
  
public class UserMethodAccess extends MethodAccess  
{  
  
    public UserMethodAccess()  
    {  
    }  
  
    /** 
     * 这个方法是主要是实现了MethodAccess 的抽象方法，来实现反射的功能   
     * @param obj  需要反射的对象 
     * @param i  class.getDeclaredMethods 对应方法的index 
     * @param 参数对象集合 
     * @return 
     */  
    public transient Object invoke(Object obj, int i, Object aobj[])  
    {  
        User user = (User)obj;  
        switch(i)  
        {  
        case 0:
            return user.getName();  
  
        case 1:  
            return Integer.valueOf(user.getId());  
  
        case 2:   
            user.setName((String)aobj[0]);  
            return null;  
  
        case 3:   
            user.setId(((Integer)aobj[0]).intValue());  
            return null;  
        }  
        throw new IllegalArgumentException((new StringBuilder("Method not found: ")).append(i).toString());  
    }  
}  
```
根据上面的代码可以看到,
UserMethodAccess 继承了 MethodAccess, 所有 MethodAccess.get() 返回的对象的类型都是 
MethodAccess 的子类,因此这里我们可以用 MethodAccess 类型的变量接受 UserMethodAccess 类型的对象 
MethodAccess access = MethodAccess.get(User.class);

invoke 方法中,先把 User 对象作为第一个参数传入,它会注入到 MethodAccess 的 user 变量中
但与 jdk 动态代理不同的是, 要调用 user.getName, setName 等方法时,不需要再通过反射去获取这些方法,
因为在 class 文件中,这些 getName() setName() 这些字节码内容已经完成了.(我们上面的是 .class文件反
编译后得到的代码,也就是说,.class文件已经有了 getName(),setName() 的字节码,不需要再用反射去获取相应
的字节码文件了)

