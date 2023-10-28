### cmake 是什么
cmake 是允许我们使用cmake的语言描述整个工程项目,最终生成makefile 进行快捷编译的工具,cmake的强大之处在于支持我们的代码跨平台编译,而 makefile 则很难做到这一点

### cmake 使用思路
构建 C/C++ 项目要解决的核心问题:
##### 编译时的依赖关系
头文件位置, 源文件位置 ,静态库和动态库的位置
* 头文件:`include_directories`函数
* 源文件可以用`aux_source_directory`函数
* 库文件`find_library` 相当于-L, `target_link_libraries` 相当于-l
* 查找文件还可以用`FILE`函数,其中函数有一个选项支持递归地从目录查找匹配文件(如`*.txt`)
##### 生成目标
生成.o文件
生成可执行文件
生成动态库
生成静态库
以及生成的位置在哪里
##### 测试文件
测试源文件一般和源文件分离,这种情况下如何对源文件进行测试



##### 源文件 aux_source_directory


cmake的
下面就通过例子循序渐进地完成cmake
##### 执行cmake命令的位置
当我们执行cmake时,会生成许多与cmake相关的文件,这些文件如果和我们的项目混杂在一起,无疑会污染整个工程,因此,一个常被建议的做法是,在工程根目录下创建CmakeList.txt , 然后创建一个build 文件夹,进入build 文件夹,执行
```bash
cmake ../
```
这样生成的关于cmake的文件就会存放在build目录下而不会污染工程源码了



```bash
set(EXECUTABLE_OUTPUT_PATH ../)  ##设置输出目录!
```
输出目录是以执行cmake命令时所在的目录为进准,比如执行
