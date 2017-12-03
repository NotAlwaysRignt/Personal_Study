<project>根元素
<modelVersion>  当前maven的版本，这个一般不用我们管

    maven的项目和我们实际的项目往往不是一一对应的关系，也就是说一个我们写了一个maven项目，不一定它就是我们最终的项目，因为maven支持模块化，也就是说一个maven项目很可能是最终项目的一个模块

<version> 版本号，一般由三个数字组成，第一个数字，大版本号，第二个数字，分支版本号，第三个数字小版本号
三个数字后一般还会附加一个单词，如 snapshot(快照，不稳定)，alpha(内测)，beta(公测), release(稳定版本)，GA正式发布

<packaging> 打包方式，不指定的话默认是jar，指定后可以打包为 zip，war，pom等


<dependencies> 所有依赖的包
<dependency>
    <groupId>
    <artifactId>
    <version>
    <scope> 可以有test，compile，比如scope内是test的时候，表示只在maven test阶段才引入此依赖，而compile阶段不会
    
    <esclusions>
        <exclusion>可以显示排除依赖，指定不引入的jar包
    </exclusions>
</dependencies>



<build> 描述的是maven构建行为,比如 install命令，它可以再被具体细分，比如可以打包成不同的格式，这时我们可以选择具体的插件，如果我们不指定，那么maven会去下载默认的插件
        <plugins>
        <plugin>
            <groupId>
            <artifactId>
            <version>
        </plugin>
    </plugins>
</build>



<dependencyManagement> 这个标签在当前maven项目中不会起作用，而会在继承这个pom.xml的子maven模块中起作用,子模块中使用<parent>标签继承父模块，就会去获取<dependencyManagement>里的信息，具体见文档
    <dependencies>
    </dependencies>
</dependencyManagement>


<parent>  用于继承另一个maven的pom.xml
<groupId></groupId>
<artifactId></artifactId>
<version></version>
</parent>


<modules>
一般在maven父模块中的pom.xml使用，这个maven父类可能本身不用来写什么代码，只是用来组织maven用，比如用下面的每一个module标签都指定一个maven项目，然后对这个maven父模块进行 clean install，那么下面所有指定的module也会被打包进本地仓库中，而不用我们一个一个地install
    <module></module>
    <module></module>
    <module></module>
    ...
</modules>

下面这些不会影响工程的结构，属于附加信息
<name> 项目描述
<url> 项目地址
<description> 项目描述
<developers> 开发人员信息
<liscenses>许可证信息
