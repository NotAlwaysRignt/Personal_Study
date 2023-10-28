### synchronized 解决的问题
synchronized 即同步,它是用来同步代码块的.其本质是借助 java的锁机制

### java 的锁  
#### java的多线程是怎么使用的?
将一个对象要使用多线程,必须继承Thread类或runnable接口,然后实现run()方法,创建这个对象,用Thread类型的对象接收它,Thread 对象调用使用start() 方法来调用实现 run方法,当一个对象传给多个Thread对象并用start()启动后,多个run()就并发执行了  

java的锁有两种,**对象锁**和**类锁**：  
对象锁是用于对象实例方法，或者一个对象实例上的,  同一个对象上被加锁的线程
类锁是用于类的静态方法或者一个类的class对象上的。
### 用法
该关键字可以修饰的范围有:**对象**和**类**,其中对象和类有分别有两种情况
1. 代码块,使得**同一个对象**中(这个对象被传入 Thread 启动)该代码块不能被多个 Thread 对象同时调用
```java
public  void run() {
	  //todo
      synchronized(this) {
         ...  //可以是一个函数内的某一块
         }
      }
```
2. 修饰一个方法,修饰方法范围是整个函数,使得**同一个对象**中(这个对象被传入 Thread 启动)该代码块不能被多个 Thread 对象同时调用
```java
public synchronized void run() {
	//todo
}
```

3.   修改一个静态的方法，其作用的范围是整个静态方法，作用的对象是**这个类的所有对象**； 
4.  修饰一个类，其作用的范围是synchronized后面括号括起来的部分，作用的对象是**这个类的所有对象**。  
```java
class ClassName {
   public void method() {
      synchronized(ClassName.class) {
         // todo
      }
   }
}
```