[TOC]
## 为什么要手动安装
官网提供了一个 `install.py` 脚本来安装 YoucompleteMe,这个`install.py`封装了很多安装过程的细节,用户只要传入参数即可,但是正如官网所说,这样安装的成功率并不高
> These instructions (using install.py) are the quickest way to install YouCompleteMe, however they may not work for everyone. If the following instructions don't work for you, check out the full installation guide.

使用`install.py`还对 python 的安装有要求,必须在安装 python 时使用`--enable-shared`,否则运行不了 `install.py`.
不过**`install.py`是非常好的安装教程**,通过阅读`install.py`,我们可以知道要让 YCM 运行起来,做了哪些编译操作,然后我们可以分别手动去做这些操作,逐步调试,直到成功安装 YCM

自己到网上下载符合当前操作系统版本的clang 二进制文件,然后再去调用 YCM 的 `CMakeLists.txt`,或者对其它语言的插件进行手动安装,有时比自动安装还会更容易

手动安装的过程[官网](https://github.com/Valloric/YouCompleteMe)已经有详细的讲解,但是即便按照官网的教程来,还是遇到了一些问题,这里讲解解决的办法,也会分析问题的原因

### 安装前的工作
建议手工源码编译好的python 和 vim,并确保 python可以启动, `vim --version`显示已经支持python3, YCM后面的编译也要和vim对应的python保持一致,才不会出错

## 手动安装过程(语义补全)讲解
以下过程重点讲述到官网下载已经编译好的 clang+llvm 二进制压缩包,并通过`cmake`等命令一步步操作安装,而不用`install.py`安装
#### 执行 ycmd/cpp/CMakeLists.txt
通过 YouCompleteMe 的`Full Installation Guide`可以知道,我们要获得`ycm_core`这个动态库(不管任何语言的补全,ycmd 作为 server 要启动必须加载它,否则无法启动),可以手动使用`cmake`编译获取它,首先是要获取 `makefile`

接着我们就可以调用`cmake`去执行`CMakeLists.txt`了,这里介绍使用自己到官网下载的二进制 clang 并进行安装,对于 centos或其他上麦安的操作系统,我们一样可以下载ubuntu的版本,验证方法是下载编译号的文件后可以进入bin目录尝试执行`./clang --version`如果能够运行起来说明二进制是兼容的
首先根据自己的操作系统(ubuntu 16)下载对应的二进制压缩包`clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz`存放到我的目录`~/Linux_application/clang/`并解压,在`~/Linux_application/clang/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04/`下可以看见`bin`,`include`,`lib`等目录

接着进入一个自定义创建的目录,用于存放cmake产生的各种文件
这里**先上结论**,最后要执行的命令格式如下:
```bash
cmake -G "Unix Makefiles" -DPATH_TO_LLVM_ROOT=~/Linux_application/clang/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04 -DUSE_PYTHON2=OFF -DPYTHON_INCLUDE_DIR=~/usr/include/python3.5m/ -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so . ~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp
```
如果不想往下看,那么就在上面的命令上修改,注意`-DUSE_PYTHON2=OFF`说明vim支持python3,可以使用`vim --version`看是否有`+python3`,`-DPYTHON_LIBRARY`要指向python3的`libpython3.xm.so`的路径,这个取决于python的 lib 安装路径,`-DPYTHON_INCLUDE_DIR`则指向安装python的头文件的路径,不同版本文件名也会不同,如果在手动安装python时,使用`./configure --prefix=mydir`
那么`mydir`目录下就会生成 `bin`,`include`,`lib`等目录,设置时要到下面寻找


#### CMakeList.txt 命令详解
根据官网,安装 YCM 需要执行以下命令,**注意官网给的命令是行不通的,我们还要做进一步的设置,后面会做讲解**
```bash
cmake -G "Unix Makefiles" -DPATH_TO_LLVM_ROOT=~/Linux_application/clang/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04 . ~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp
```
`-G`后跟的参数指定生成的文件,使用`cmake --help`可以看选项,在Linux下使用`"Unix Makefiles"`即可(记得加双引号,不然 Unix Makefiles的 Unix 会被单独拿出来解析),表示`Generates standard UNIX makefiles.`

`-DPATH_TO_LLVM_ROOT`指定我们下载的二进制文件存放目录,cmake的最后一个参数是`ycmd/cpp`下的 `CMakeLists.txt`,注意官网用的是`~/.vim/bundle/YouCompleteMe/third_party/ycmd/cpp`,不要一味照抄,要根据实际情况设置.本人使用`plug-vim`下载的YCM,因此YCM插件安装在`~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp`目录下

**成功编译可用的`ycm_core.so`文件是YCM安装的重难点,如果成功,那么可以说完成了90%的安装**
YoucompleteMe在启动时会通过python启动一个后台进程,名为ycmd,它是一个http server服务,我们在vim中编辑代码时,会作为client 不断把代码的信息作为 Json 串发送HTTP信息给 ycmd server
ycmd拿到代码信息后,会将其进行编译,解析,然后将补全结果返回.可见,如果 ycmd 不能启动,那么 YCM 就用不了了,而 ycmd 要启动,必须要有可用的`ycm_core`动态链接库

要获取`ycm_core`动态库文件,**关键就是要用`cmake`成功执行`YouCompleteMe/third_party/ycmd/cpp/`下的`CMakeLists.txt`并用`cmake --build`(或者 make )将其成功编译出来**

事实上根据官网直接这样编译还是会有问题的,当我们执行 cmake 时会发现以下打印
```bash
-- Found PythonLibs: /usr/lib/x86_64-linux-gnu/libpython2.7.so (found suitable version "2.7.12", minimum required is "2.7")
```
说明它会编译生成python 2 版本的`ycm_core`,而我的vim是支持 python3的,如果编译 python2 的ycm_core, vim 就会无法载入`ycm_core`
另外一个问题是,假如要指定生成 python3 版本的 ycm_core,cmake在查找python库时必须要查找编译时使用了`-fPIC`的 python3 库,否则编译`ycm_core.so`时也会失败

下面分别讲解如何解决这两个问题
#### 指定编译 python3 版本的 ycm_core
解决这个问题的办法是传入`USE_PYTHON2`参数,设置为OFF
这个值默认被设为了ON (在`YouCompleteMe/third_party/ycmd/cpp/CMakeLists.txt`中设置了`option( USE_PYTHON2 "If on, link to Python 2 instead of 3" ON )`)
这个是读`CMakeLists.txt`查询得到的办法,官网的`README.md`并没有提到这个问题
```bash
cmake -G "Unix Makefiles" -DPATH_TO_LLVM_ROOT=~/Linux_application/clang/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04 -DUSE_PYTHON2=OFF . ~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp
```
发现显示的信息中提示`-- Found PythonLibs: /usr/local/lib/libpython3.6m.a (found suitable version "3.6.1", minimum required is "3.4")`
说明将会编译 python3 版本的ycm_core,**但是这还是有问题的**

#### 设置 PYTHON_LIBRARY 变量
上面提示信息中`libpython3.6m.a`,这是一个静态库,编译`ycm_core.so`的过程需要使用动态链接,静态库显然不可以作动态链接,因此如果就此往下编译,会提示编译失败,需要一个`-fPIC`编译的库
YoucompleteMe 中`README.md`的`FAQ`提到了这个问题并给出了解决办法
> Use the -DPYTHON_LIBRARY= CMake flag to point it to a .so version of libpython on your machine (for instance, -DPYTHON_LIBRARY=/usr/lib/libpython2.7.so).

为什么要这样操作?我们可以在`YouCompleteMe/third_party/ycmd/cpp/CMakeLists.txt`在中得知查找 python 库使用的是`cmake`的命令是`find_package( PythonLibs 3.4 REQUIRED )`

PythonLibs是 cmake自带的模块脚本,进入 cmake 的官网,在`FindPythonLibs`章节,我们可以查到,在执行`find_package`时,我们也可以手动传入变量指定 python 库和头文件的位置
```
PYTHON_LIBRARY             - path to the python library
PYTHON_INCLUDE_DIR         - path to where Python.h is found
```

找到python3 的动态库可以在linux下只用locate命令
`locate libpython | grep 'so'`
在操作系统中我找到了其中一个符合条件的动态库存放于`/usr/lib/x86_64-linux-gnu/libpython3.5m.so`中,于是最终我们执行
```bash
cmake -G "Unix Makefiles" -DPATH_TO_LLVM_ROOT=~/Linux_application/clang/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04 -DUSE_PYTHON2=OFF -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so . ~/.vim/plugged/YouCompleteMe/third_party/ycmd/cpp
```
发现提示信息`-- Found PythonLibs: /usr/lib/x86_64-linux-gnu/libpython3.5m.so (found suitable version "3.6.1", minimum required is "3.4")`且没有报错
之所有3.6.1 是我用ubuntu用`apt-get`后面装上的,python3.5是系统自带的版本

#### cmake --build
如果前面的安装没有报错,成功执行 cmake 后,接下来就是执行`Makefile`了,执行如下命令
```bash
cmake --build . --target ycm_core --config Release
```
`--config Realease`是在 Windows 系统下起作用的,Linux 下会被忽略,`cmake --build .`相当于在当前目录下执行 `make`,`ycm_core`就是我们要编译的目标文件

编译成功后,会提示我们生成`ycm_core.so`存放在`~/.vim/plugged/YouCompleteMe/third_party/ycmd/ycm_core.so`目录下
**生成`ycm_core.so`后,我们可以删除网上下载的 llvm+clang 二进制文件,因为ycmd要加载的只是`ycm_core.so`**
-----------------------------------------------------

### 正则表达式模块优化支持(可选,建议安装)
官网原话
> Build the regex module for improved Unicode support and better performance with regular expressions. The procedure is similar to compiling the ycm_core library:

`ycmd/third_party/cregex`目录下存放了正则表达式模块代码,是用 C 实现的,且分 python2 和 python3 版本,将其编译为动态库,相较于 YCM 使用 python 解析,可以提高正则表达式解析时的性能.同时根据作者所说,可以改善对 Unicode 编码的支持.
**个人还是建议编译这个库,因为代码量一大,python的性能也是个问题,使用链接动态 C 库可以大大提升性能**
`ycmd/third_party/cregex`目录下的`CMakeLists.txt`只有一百行,阅读后可知,它会调用`find_package( PythonLibs )`来查找本机的 python 库,并且区分 python2 和 python3 版本
本人的 vim 是支持 python3 的,默认会找到 python2 的版本,解决问题的办法和编译`ycm_core`的思路一样,先用locate找到 python3 的 so 动态库,然后传递变量值`PYTHON_LIBRARY`告诉 cmake 
```bash
mkdir regex_build # 任意位置创建目录,名字也可以自定义,用于存放 cmake 产生的临时文件
cd regex_build
cmake -G "Unix Makefiles" -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so . ~/.vim/plugged/YouCompleteMe/third_party/ycmd/third_party/cregex
cmake --build . --target _regex --config Release  # 相当于执行make
```

思路和前面`ycm_core`是类似的,且正则表达式模块的编译没有`ycm_core`的编译那么复杂,因为主要是对 c 源码的编译,不涉及链接一些二进制库

成功编译时会提示
```bash
[100%] Linking C shared library /home/thinker/.vim/plugged/YouCompleteMe/third_party/ycmd/third_party/cregex/regex_3/_regex.so
[100%] Built target _regex
```
生成`_regex.so`文件存放在`/YouCompleteMe/third_party/ycmd/third_party/cregex/regex_3/`


### .ycm_extra_conf.py 配置
#### `.ycm_extra_conf.py`的功能
YCM可不会解析`makefile`,它不知道我们的编译 flag 是什么样,自然也不知道我们是不是使用 -I 加载了别处的头文件,为了解决这个问题,YCM要求我们自己编写一个`.ycm_extra_conf.py`,以告诉YCM这些信息,如果YCM 找不到这个文件,那么无法提供语法补全
具体的配置使用请见本人github的另一篇关于`.ycm_extra_conf.py`的说明


-------------------------------------------------------

## YCM安装过程如何调试
当成功编译生成`ycm_core`以后,我们需要在启动时得到 YCM 的报错信息,才能知道YCM是否成功启动了,这里介绍获取这些信息的手段
Vim中输入`:YcmDebugInfo`,回车得到如下调试信息 (输入`:Ycm`然后按多次`<tab>`键可以补全得到这个命令)
```bash
Printing YouCompleteMe debug information...
-- Client logfile: /tmp/ycm_02xrmjzw.log
-- Server errored, no debug info from server
-- Server running at: http://127.0.0.1:49977
-- Server process ID: 7991
-- Server logfiles:
--   /tmp/ycmd_49977_stdout_u3u23057.log
--   /tmp/ycmd_49977_stderr_qivafm3t.log
```
YCM 若成功启动,则后台应该有一个`ycmd`进程启动,通过`ps -ef | grep Pid`可以查看这个`ycmd`进程是不是启动了,如果没有找到这个进程,说明ycmd 启动失败
通过输出信息可知,错误信息被存放到了`/tmp/ycmd_49977_stderr_qivafm3t.log`目录中,我们通过这个文件就可以知道出错的问题是什么

举例:
```bash
$ cat /tmp/ycmd_49977_stderr_qivafm3t.log
2019-01-16 23:10:27,840 - ERROR - ycm_core library compiled for Python 2 but loaded in Python 3.
Traceback (most recent call last):
  File "/home/thinker/.vim/plugged/YouCompleteMe/third_party/ycmd/ycmd/server_utils.py", line 97, in CompatibleWithCurrentCore
    ycm_core = ImportCore()
  File "/home/thinker/.vim/plugged/YouCompleteMe/third_party/ycmd/ycmd/server_utils.py", line 89, in ImportCore
    import ycm_core as ycm_core
ImportError: dynamic module does not define module export function (PyInit_ycm_core)
```

如果成功启动了ycmd,在输入`:YcmDebugInfo`时,可以看到如下信息
```bash
Printing YouCompleteMe debug information...
-- Client logfile: /tmp/ycm_3avqtjjf.log
-- Server Python interpreter: /usr/local/bin/python3
-- Server Python version: 3.6.1
-- Server has Clang support compiled in: True
-- Clang version: clang version 7.0.1 (tags/RELEASE_701/final)
-- No extra configuration file found
-- Server running at: http://127.0.0.1:44121
-- Server process ID: 10264
-- Server logfiles:
--   /tmp/ycmd_44121_stdout_rnebiy78.log
--   /tmp/ycmd_44121_stderr_mdmvvo16.log
```

根据提示信息用浏览器访问`http://127.0.0.1:44121`是时,它会返回一个json串,说明进程确实在运行
同样我们可以根据提示信息查看运行的进程
```bash
ps -ef | grep 10264
thinker  10264 10263  0 00:34 ?        00:00:00 /usr/local/bin/python3 /home/thinker/.vim/plugged/YouCompleteMe/python/ycm/../../third_party/ycmd/ycmd --port=44121 --options_file=/tmp/tmp2j_rl9zj --log=info --idle_suicide_seconds=1800 --stdout=/tmp/ycmd_44121_stdout_rnebiy78.log --stderr=/tmp/ycmd_44121_stderr_mdmvvo16.log
```

### 修改 YCM 源码,解决字符解码的问题
由于系统是中文环境,安装完`Youcompleteme`进入 vim 后
发现报错
```bash
UnicodeDecodeError: 'utf-8' codec can't decode byte 0xcf in position 2: invalid continuation byte
```
现象是command窗口一直报上述错误,而且输入字符也不会弹出补全窗口
使用上面提到的日志查看方法,发现日志里面提示是因为异常抛出导致的错误
```bash
File "/home/user/.vim/plugged/YouCompleteMe/third_party/ycmd/ycmd/handlers.py", line 100, in GetCompletions
.ComputeCandidates( request_data ) )

File "/home/user/.vim/plugged/YouCompleteMe/third_party/ycmd/ycmd/completers/cpp/clang_completer.py", line 487, in BuildExtraData
if completion_data.DocString():
```
没办法,只有强行去查看源码,发现completion_data.DocString()调用的还是C++库的API,它并不是python实现的,从代码的角度看,`DocString`也不涉及代码,于是我便修改了源码,将
```python
   if completion_data.DocString():
      extra_data[ 'doc_string' ] = completion_data.DocString()
```
修改为
```python
  try: # 这里可能抛异常,自己补上
    if completion_data.DocString():
      extra_data[ 'doc_string' ] = completion_data.DocString()
  except UnicodeDecodeError as e:
    pass
```
问题就解决了

## 其它语言补全
### GO 语义补全支持
#### gocode/godef 支持
要让 YCM 支持 GO 的语义补全,前提是把`ycm_core.so`编译好,放在`XXX/YouCompleteMe/third_party/ycmd`目录下,这样 ycmd server 才可以启动
如果没有为 YCM 安装 gocode 和 godef,则只能进行 ID 补全,即对项目输入过的单词进行补全,且不能支持语义级别的补全
YCM 对 Go 的补全和对 CPP 补全一样,我们在 VIM 中编辑时把代码信息发送给 ycmd 这个 http 服务器解析, ycmd 把补全信息返回,不过对 Go 补全时, ycmd 需要使用 Gocode 这个工具来解析 Go 代码.
gocode 和 godef 是用于解析 Go 代码的程序,其 github 仓库分别是[mdempsky/gocode](https://github.com/mdempsky/gocode)和[rogpeppe/godef](https://github.com/rogpeppe/godef),我们在下载 YCM 插件时,也会把 gocode 和 godef 仓库的代码下载下来(YCM会把各个语言支持的工具的仓库都下载下来,不管我们用不用,因此 YCM 的插件非常大)
由上可得,要让 YCM 支持 GO 补全,关键是让 ycmd 能够调用 gocode 和 godef

### 手动使 YCM 支持 Go 补全
#### gopls补全
gopls是golang 推出的go补全工具,比gocode更加先进, 要注意的是我们要**先安装 ctags** 建议使用`univesal ctags`,这个版本比较新,运行`autogen.sh`,然后就是`./configure`三步走
手动安装 gopls 不难,首先要安装好 go,建议使用 go.11 以上的版本,支持 go module
然后按照官网`Full Installation Guide`讲解,进入`YouCompleteMe/third_party/ycmd/third_party/go/src/golang.org/x/tools/cmd/gopls`目录并运行`go build`即可

#### gocode 补全工具安装
目前YCM 已经不使用 gocode了,使用gocode 补全比较麻烦,因为它分支持 gopath 和 不支持 gopath 两个版本,但是如果安装了一些老版本,这里还是讲解一下踩过的坑
要弄懂手动安装的过程,可以通过阅读`YouCompleteMe/install.py`的安装方法

`install.py`在后面讲解,这里**直接上结论**,要支持go补全,需要如下操作
* 修改 GOPATH,将`YouCompleteMe/third_party/ycmd/third_party/go`作为 GOPATH,如`export GOPATH=~/.vim/plugged/YouCompleteMe/third_party/ycmd/third_party/go:$GOPATH`,如果不设置,会编译不通过,只要临时设置即可,编译通过后可去掉这个GOPATH
* 分别到`ycmd/third_party/go/src/github.com/mdempsky/gocode`和`ycmd/third_party/go/src/github.com/rogpeppe/godef`下执行`go build`命令

#### `mdempsky/gocode`注意事项
`mdempsky/gocode`是 YCM 默认采用的 gocode 版本,它只支持`$GOPATH`类型项目,而不支持`go module`类型项目,一个值得注意的一点是:
对于自己编写的自定义包,需要`go install`这个包.才能对自定义的包进行补全,而第三方包因为执行`go get -u`时已经进行了`go install`,就不用了
因此写代码的过程中,如果自定义的包也要补全,需要定时对这个包`go install`
`stamblerre/gocode`不会有此问题

#### 使用`stamblerre/gocode`
**当前(2019-1-20)YCM的 go 补全采用的是`mdempsky/gocode`,其官方文档告诉我们,这个版本的 gocode 并不支持go module项目的补全**
> It only works for $GOPATH projects. For a version of gocode that works with Modules, please see github.com/stamblerre/gocode.
YCM 并没有支持`stamblerre/gocode`,根据作者 stamblerre 的说法,Go团队正在开发`golang.org/x/tools/cmd/golsp`,这将最终称为 gocode 的替代品,同时作者也在找机会将两个 gocode 项目合并.

如果我们希望使用`stamblerre/gocode`,则可以使用 YCM 提供的`g:ycm_gocode_binary_path`选项,YCM 的文档并没有这个选项的解释,但是我们如果在仓库中搜索`gocode`关键字是可以找到这个选项的,默认为空字符串,如果这个值被设置,就会使用该选项指向的`gocode`可执行文件,否则默认使用 YCM 项目中下载编译的`gocode`

因此我们可以手动到github上下载`stamblerre/gocode`并编译生成可执行文件,然后用`g:ycm_gocode_binary_path`指向这个可执行文件的位置,注意应该使用绝对路径,我们通过`YcmDebugInfo`中找到错误日志的位置并打开查看是否设置成功
`stamblerre/gocode`依赖了`golang.org/x/tools`包,我们在国内因为网络不通无法下载这个包,不过 github 中有 golang.org 代码的镜像,我们可以先到 github 上用`git clone`把 `golang/tools` 下载下来.接着在`$GOPATH/src`下创建`golang.org/x`两个目录,并用`ln -s`创建`golang.org/x/tools`这个软链接指向`tools`这个github 项目,这样操作后,`stamblerre/gocode`就可以编译通过了,我们在`$GOPATH/bin`下可以找到`gocode`,在vim中就可以设置指向它
#### 分析 install.py 源码了解 go 的安装步骤
接着来讲解一下`install.py`,首先看文档说法
> Go support: install Go and add --go-completer when calling install.py.

因此只要知道`install.py`是如何处理`--go-completer`,就可以知道如何安装 go 补全.`install.py`的功能主要就是检查一下 python 版本然后调用`YouCompleteMe/third_party/ycmd/build.py`,真正的安装逻辑就是这个`build.py`这个目录,我们通过解读它可以知道 go 补全插件是被如何支持的

`build.py`的源码可读性很强,顺着 go 的逻辑往下读
```python
if __name__ == '__main__':
  Main()

def Main():
  args = ParseArguments()
  cmake = FindCmake()
  cmake_common_args = GetCmakeCommonArgs( args )
  # ......
  if args.go_completer or args.gocode_completer or args.all_completers:
    EnableGoCompleter( args )
  # ......

def EnableGoCompleter( args ):
  go = FindExecutableOrDie( 'go', 'go is required to build gocode.' )

  go_dir = p.join( DIR_OF_THIS_SCRIPT, 'third_party', 'go' )
  os.chdir( p.join( go_dir, 'src', 'github.com', 'mdempsky', 'gocode' ) )
  new_env = os.environ.copy()
  new_env[ 'GOPATH' ] = go_dir
  CheckCall( [ go, 'build' ],
             env = new_env,
             quiet = args.quiet,
             status_message = 'Building gocode for go completion' )
  os.chdir( p.join( go_dir, 'src', 'github.com', 'rogpeppe', 'godef' ) )
  CheckCall( [ go, 'build' ],
             env = new_env,
             quiet = args.quiet,
             status_message = 'Building godef for go definition' )

```
代码写得很清楚,先检查 go 是否安装,然后分别到`ycmd/third_party/go/src/github.com/mdempsky/gocode`和`ycmd/third_party/go/src/github.com/rogpeppe/godef`下执行`go build`命令
#### 调试 Go 语义补全
调试方法和调试`ycm_core`类似,就是查看`ycmd`输出的错误信息,输入`:YcmDebugInfo`,
```bash
-- Server logfiles: # 得到 YCM 打印日志的位置
--   /tmp/ycmd_34062_stdout_g6ycmhyh.log
--   /tmp/ycmd_34062_stderr_yfmaf46k.log
```
接着用 cat 查看日志就知道出错在哪里了,比如在没有编译 gocode 和 godef 时得到错误信息
```bash
ValueError: No semantic completer exists for filetypes: ['go']
```

### Python语义补全
YCM 补全 python 基于 jedi 引擎,jedi在 YCM 通过`vim-plug`安装时也会被安装好,并且我们不需要再做额外的安装操作
对于普通的python系统库补全,我们不再需要做设置,但是如果使用了虚拟环境,就要专门写`.ycm_extra_conf.py`文件了,官网给出了一个简单例子
```
def Settings( **kwargs ):
  return {
    'interpreter_path': '/path/to/virtual/environment/python'
  }
```
官网对`/path/to/virtual/environment/python`的解释是:python虚拟环境的目录,对于windows,这个目录有一个`Script`目录,里面应该存放着 python 的可执行文件,对于其它平台,这个目录下应该有一个bin目录,python 的可执行文件存放在bin目录下
#### 注意g:ycm_filetype_whitelist
如果我们要用`g:ycm_filetype_whitelist`设置白名单,注意key值是vim的filetype.对于python文件对应`python`而不是`py`,这点可以打开一个`.py`文件然后使用`:set filetype`查看,如果设置错了, YCM 就不会进行补全了
#### 获取 python 补全引擎调试信息
打开`.py`文件,输入`:YcmDebugInfo`查看是否补全成功
