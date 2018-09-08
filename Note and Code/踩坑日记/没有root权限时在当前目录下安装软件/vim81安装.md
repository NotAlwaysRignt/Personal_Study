### 在开发机上安装VIM 8.1(搭建属于自己的超强IDE)
> 公司的开发机gcc版本老旧，而且不支持连接外网，也不支持把软件放在/usr目录下，因为没有用户权限，所以软件的安装非常不方便，只能通过源码安装属于自己的vim，期间踩坑无数，这里记录，也为日后手动安装linux软件提供思路

##### 下载vim源码
先到github下载源码，拷贝都开发机上(如果不能直接传到开发机上，就先拷贝到跳板机上，再传到开发机上) 解压后进入目录 

##### 编译VIM8.1 并且使其支持 python3.6  
我们需在./configure 进行一些设置支持即可 
以下的配置让vim支持python，所以请确保python 3.6已经安装好并且放置在指定的目录下(源码安装python3.6时 ./configure --prefix=/data/home/chaoranxu/app/bin)
```bash
./configure --prefix=/data/home/chaoranxu/app/bin/ --enable-python3interp=dynamic  --with-python3-config-dir=/data/home/chaoranxu/bin/lib/python3.6/config-3.6m-x86_64-linux-gnu
```

**在安装前修改vim的源代码，解释在后面**

接着
```bash
make
make install
```
安装完成，可以使用 vim --version ，发现版本是8.1，找到vim选项并且显示 **+python3/dyn**
说明安装成功
注意 +python3 后面必须显示 dyn，否则会出现问题，这是开发机环境决定的

### 出现的问题以及解决办法
#### 1.安装vim后没有将可执行文件安装到指定目录下
这个问题的原因是什么暂时还不知道，只能详细读懂makefile才能知道原因了，但是如果在执行make 和make install都梳理进行的话，即此时我们已经成功了编译了vim，那么就一定会在某个位置存放了vim的可执行文件。
这时候我们可以采用 alias 的方式指定vim
我们可以先用 find -name vim 在当前目录下找到编译生成的可执行文件,这个文件一般放在源码包的src目录下,找到后我们就可以指向它
修改 .bashrc 文件，添加如下一句
```bash
alias vim="~/app/vim-8.1.0178/src/vim"
```
这样,调用 vim 时,就会默认执行 ~/app/vim-8.1.0178/src/ 下的vim 了

#### 2.无法启动vim,启动即崩溃
启动vim时显示
```bash
Fatal Python error: PyThreadState_Get: no current thread
Vim: Caught deadly signal ABRT
Vim: Finished.
```
这个bug 搜了好久都没有找到答案:
最后找到这篇文章解决了: 
https://github.com/boxu-zhang/vim  

大致原因就是如果我们安装了多个版本的python,产生了库的冲突,解决办法如下:
在源码编译 vim 时,在使用./configure 选项时,添加(这里我要编译的vim是支持python3的,如果是支持python2,就用--enable-pythoninterp=dynamic)
```bash
--enable-python3interp=dynamic
```
这样编译vim并使用 make make install后
执行
```bash
vim --version
```
我们将看见
**+python3/dyn** 这一项,注意如果没有 dyn 则说明失败了

如果还是不行,我们要修改源代码的文件
进入vim源码压缩包解压后的src目录下,修改if_py_both.h 文件
```cpp
// Disabled, because this causes a crash in test86
// OutputType.tp_base = &PyFile_Type;

修改为
// Disabled, because this causes a crash in test86
OutputType.tp_base = &PyFile_Type;
```
很显然,注释都让我们不要这样改了,这是作者的建议,在开发机上我并没有那么做,只是修改vim编译时的选项

#### 安装 git
如果我们使用 vim-plug 插件,它会默认把插件安装到 ~/.vim/plugged 目录下,要在启动vim时也启动 vim-plug 安装的插件,需要我们安装Git,否则会使用不正常