cmake的变量使用 ${变量名} 获取,但在if语句中不用 ${},而是直接使用变量名.
cmake支持我们自定义变量,同时也有自己的一些具有特殊意义的变量,下面先介绍关于变量的一些操作
### 打印变量
打印变量是很重要的,这让我们可以看到变量是不是被正确设置了,cmake提供了 message 函数来打印变量语法如下
```bash
message([SEND_ERROR | STATUS | FATAL_ERROR] "message to display" ...)
```
SEN_ERROR 等必须大写
使用 message 后,变量的打印将在执行cmake时执行
* FATAL_ERROR 和SEND_ERROR 都是错误关键字
* STATUS 输出正常
* WARNING 警告

```bash
message(STATUS "ouput status")
```
执行cmake时将看见
```bash
-- ouput status
```

### 设置变量
我们可以通过 set 来设置变量,set(变量 变量值)的形式
```bash
set(EXECUTABLE_OUTPUT_PATH ../)
```
这样,${EXECUTABLE_OUTPUT_PATH} 的值即为 ../  ,关于变量我们还需要记住几个常用的隐式变量,这些变量有特殊的含义

### cmake 的特殊变量

##### EXECUTABLE_OUTPUT_PATH
可执行文件的输出路径,设置此变量后,我们最终生成的可执行文件就会生成到这个变量指定的目录下,上面的set(EXECUTABLE_OUTPUT_PATH ../), ../依据的是执行cmake命令时的路径

##### 工程根目录
* CMAKE_SOURCE_DIR 
* PROJECT_SOURCE_DIR 
* < projectname >_SOURCE_DIR

上面三个变量我们可以视为相同,都是最开始执行的 CmakeList.txt所在的位置,一般是存在在工程的根目录中

关于< projectname >_SOURCE_DIR  
< projectname > 是通过project函数设置的,举例如下:
```bash
project(hello)
message(STATUS "project: ${hello_SOURCE_DIR}")
```
执行cmake时将看到打印
```bash
-- project: /home/thinker/桌面/测试用例/C-family/CmakeTest/cmakeVariable
```