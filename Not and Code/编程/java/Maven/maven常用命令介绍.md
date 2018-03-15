-mvn compile 对项目里的代码进行编译(前提是我们的项目各个目录已经布置好,pom.xml也已经写好了)
这一步会在src下生成一个target文件,内部包含了由源代码编译生成的class文件,测试报告等


-mvn test 运行 src/test/java中的测试代码


-mvn package 打包项目为一个jar包


-mvn clean  将src下的target目录删除,也就是把target中的编译好的class文件,测试报告等删除了


-mvn install 安装jar包到本地仓库中
举例: 假如我们一个maven项目放在 mv1 文件中,进入 mv1,它的结构如下:  
sc  
　　-main  
　　　　-java  
　　　　　　-package  
　　-test  
　　　　-java      
　　　　　　-package  
　　-resources  
pom.xml

我们进入 mv1 后,在命令行输入 mvn install,等待完成,这时这个mv1这个项目就会被打包成jar包放进本地仓库中,假设我们在另一个项目文件mv2中要导入mv1中的类,如果不用maven,我们可以把mv1打包成jar包,然后在IDE中设置将这个jar包添加到 classpath 中, 如果利用了maven,我们只需要在mv2的 pom.xml文件中添加mv1的 &lt;dependency&gt;标签,这样我们要用到的 mv1的类就会到本地仓库中加载了


-mvn archetype:generate
利用原型创建目录结构,比如source目录,存放代码目录等等的布局,帮我们把目录布好局
