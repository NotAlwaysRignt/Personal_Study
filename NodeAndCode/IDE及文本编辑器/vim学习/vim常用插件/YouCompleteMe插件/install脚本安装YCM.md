# YCM(YouCompleteMe) 安装教程
#### 安装前置要求
YCM 的安装还有一些依赖,要先提前安装,官网有介绍
```bash
# 以下是ubuntu16.04 的依赖,fedora 和 ubuntu14 有所不同
sudo apt install build-essential cmake python3-dev
```
YCM 通过 python 来运行作为 vim 插件,因此需要 python 环境, YCM 下载过程要编译 clang 库,需要用到 cmake

#### vim-plug下载
进入github YouCompleteMe 的官网,使用 plug 插件下载
```bash
# .vimrc
Plug 'Valloric/YouCompleteMe'
```
在 vim 中执行`:PlugInstall` 安装

#### 使用install.py 安装
**官网中的文档讲得很详细,对非C家族语言调用 install.py 脚本时要加相应参数,建议对着官网最新的文档阅读并操作**
通过 vim-plug 安装 YCM 后,安装并未结束, YCM 借助了 clang 这个工具,它需要到网上下载 clang 库,并对它进行编译生成动态库,之后 YCM 插件里的 python 代码会去调用这个库的接口,来执行补全(python 调用C++库)


#### install.py 安装过程
进入YCM插件目录,如果是用 vim-plug 安装,则插件存放在 `~/.vim/plugged/YouCompleteMe`目录下,`install.py`用于执行安装的文件
`install.py` 主要完成的功能是,检查是否已经安装了 cmake ,如果安装了就用 cmake 去执行指定目录下的 `CMakeLists`,这个过程会去网络上下载 clang 库并编译(这个过程很容易出错),生成动态库
```bash
python3 install.py # 只安装C家族的补全插件,而且不会有语义补全支持
python3 install.py --clang-completer # 只安装C家族的补全插件,有语义补全支持
```

YouCompleteMe 除了 C 家族的补全支持,还支持其它语言的支持,比如 Go, Js, Java 等,**要支持其它语言,要在执行`build.py`脚本时加上相应的选项**,下面是官网的话,安装时具体见官网
* C# support: install Mono with Homebrew or by downloading the Mono Mac package and add --cs-completer when calling install.py.
* Go support: install Go and add --go-completer when calling install.py.
JavaScript and TypeScript support: install Node.js and npm and add --ts-completer when calling install.py.
* Rust support: install Rust and add --rust-completer when calling install.py.
* Java support: install JDK8 (version 8 required) and add --java-completer when calling install.py

这里我除了要支持 C 家族语义补全,还要让它支持 `Go` 和 `Javascript` (Java还是决定交用 IDEA 了..)
执行
```bash
python3 install.py --clang-completer --go-completer --ts-completer
```

遇到的问题:
```bash
ERROR: found static Python library (/usr/local/lib/python3.6/config-3.6m-x86_64-linux-gnu/libpython3.6m.a) but a dynamic one is required. You must use a Python compiled with the --enable-shared flag. If using pyenv, you need to run the command:
  export PYTHON_CONFIGURE_OPTS="--enable-shared"
before installing a Python version.
```
默认的 ubuntu 在安装 python3 时没有 --enable-shared 这个选项,终端输入`vim --version`来查看 vim 的 python 支持时,显示的是`+python3`
https://www.mobibrw.com/2018/10071
```bash
export PYTHON_CONFIGURE_OPTS="--enable-shared"
```

YouCompleteMe 调用 `build.py`时,一个很耗时的的安装过程在于通过 cmake 执行 `CMakeLists.txt` 到网络下载clang 库(cmake 中的 DOWNLOAD 函数可以连网下载),下载完毕后还要最终编译成一个动态库(由 CMakeLists.txt可知名为ycm_core,后缀为pyd,或.dll或.so),clang 库很大,因此这个过程会比较长.
编译生成动态库文件是至关重要的一步, YCM 就是通过 python 调用这个动态库来做语法,词法解析,并获得补全信息的

#### YCM 执行原理简介
YCM 插件的执行过程比较复杂,在启动 vim 时, YCM 会先用 python 启动一个后台进程,这是一个网络服务,相当于服务端
我们在 vim 中写代码时, YCM 会把代码信息通过 HTTP JSON 的格式发送给服务端进程,进程会通过 python 去调用 clang 库的解析补全接口来进行 语义补全,最后把补全信息返还给作为客户端的我们
