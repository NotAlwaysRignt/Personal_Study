搭建JAVA开发环境并不复杂,下载完JAVA后,只需要设置以下变量,即可以让Java跑起来
* JAVA_HOME,安装的java目录,这个目录下有bin目录,有jre 目录,有lib 目录,很好认
* JRE_HOME:  export JRE_HOME=$JAVA_HOME/jre
* CLASSPATH:  export CLASSPATH=$JAVA_HOME/lib:$JRE_HOME/lib:$CLASS_PATH
* PATH:  export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH

(**注意CLASSPATH,CLASS 和 PATH 间没有下划线**)

##### 遇到的问题及解决办法
测试机上已经安装了 java 1.8,输入 java -version 可以显示版本,用javac 也可以编译java文件,但是在运行class时却一直报错
```bash
错误: 找不到或无法加载主类 HelloWorld
```
这个问题网上一搜答案挺多,最后解决办法如下:
能用javac 编译,说明 JAVA_HOME 肯定已经设置成功了,找不到主类的原因是 CLASSPATH 设置有误,这里我的测试机上就没有设置
```bash
echo $JAVA_HOME
```
看一下所在路径

接着只要根据JAVA_HOME 设置 JRE , CLASSPATH 以及 PATH 路径 
```bash
export JAVA_HOME=/usr/local/services/java_list_server-1.0/jdk1.8.0_144
export JRE_HOME=$JAVA_HOME/jre
export CLASSPATH=$JAVA_HOME/lib:$JRE_HOME/lib:$CLASS_PATH
export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
```
设置完后,写一个HelloWorld.java,果然运行起来了

另外遇到这种问题还有一个很大的可能是package 关键字的原因,关于使用package后要如何运行编译好的class文件,请到网上查找相关资料