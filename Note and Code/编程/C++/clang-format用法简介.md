### clang-format 介绍
clang-format 是代码格式化工具,它支持多种语言的格式化,如`C/C++`,`Java`,`Javascript`等. 它既提供了一套格式化的规范,也支持我们在格式化规范的基础上使用自定义的方式,非常方便

### clang-format安装
我们可以使用系统安装工具`yum`或者`apt-get`进行安装,也可以到 clang 官网,直接下载对应平台的编译好的二进制文件,二进制包下的bin目录里就有 `clang-format`,添加到`$PATH`即可

### clang-format使用
使用`clang-format -help`可以看各种选项,下面只介绍基本用法
编写一个文件`test.cpp`,使用命令
```bash
clang-format -style=LLVM test.cpp
```
可以看见在命令行中打印出了格式化后的代码,但是这个时候文件并没有被改变

`-style`可以设置格式化的风格,我们使用`clang-format`可以看到说明
```bash
  -style=<string>           - Coding style, currently supports:
                                LLVM, Google, Chromium, Mozilla, WebKit.
                              Use -style=file to load style configuration from
                              .clang-format file located in one of the parent
                              directories of the source file (or current
                              directory for stdin).
                              Use -style="{key: value, ...}" to set specific
                              parameters, e.g.:
                                -style="{BasedOnStyle: llvm, IndentWidth: 8}"
```
默认支持`LLVM, Google, Chromium, Mozilla, WebKit`这些风格,如果我们使用`style=file`,则它会默认去查找当前目录下的`.clang-format`文件,读取里面的配置.如果当前目录下找不到`.clang-format`,则会递归地项父目录查找
关于`.clang-format`文件的说明,网上有很多说明,这里不再展开

上述命令`clang-format -style=LLVM test.cpp`会在命令行里打印出,但文件没有改变.
如果我们想要改变文件,则可以加上`-i`选项`clang-format -i -style=LLVM test.cpp`

#### 编写.clang-format文件
`LLVM``Google`这些风格默认都是以两个空格为缩进的,如果我们想要4个空格缩进,但是其他又想用原来的配置,则可以编写文件如下
```bash
# .clang-format,使用4个空格缩进,其他保持LLVM的风格
BasedOnStyle: LLVM
IndentWidth: 4
```
保存,然后在当前目录下执行`clang-format -i -style=file test.cpp`,即可发现缩进规则改变了

#### 直接在style中设置
上述的`.clang-format`很短,只有两行配置,这种情况我们没必要在项目中引入一个文件,根据上述`-style`的说明,直接在`-style`中设置即可,help的说明已经说得很清楚了
```bash
clang-format -i -style="{BasedOnStyle: llvm, IndentWidth: 4}" test.cpp
```

### 编写clang-format的脚本
前面的文件都是围绕`test.cpp`一个文件,现在我们想要两个文件
```bash
clang-format -i -style=file ./*.cpp
clang-format -i -style=file ./*.h
clang-format -i -style=file ./mydir/*.cpp
# ...other
```
保存为`.format.sh`,和`.clang-format`在同一个目录下,然后运行`sh build.sh`,就可以读取`.clang-format`里的配置去格式化代码了,非常强大