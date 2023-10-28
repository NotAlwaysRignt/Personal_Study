### 在开发机上安装VIM 8.1
> 如果在机器不能连网,或者没有root 权限(不能将软件安装到/usr目录下),则不能直接用 apt-get 那样的命令安装 vim，只能通过源码安装属于自己的vim，期间踩坑无数，这里记录，也为日后手动安装linux软件提供思路

##### 下载vim源码
先到github下载源码，解压后进入目录 

##### 编译VIM8.1 并且使其支持 python3.6  
**在安装前修改vim的源代码，操作说明在后面**
源码改好了,接下来就是编译安装了
我们需在./configure 进行一些设置支持即可 
以下的配置让vim支持 python3，所以请确保 python3 已经安装好并且放置在指定的目录下(这里我用的是python3.6,源码安装python3.6时 ./configure --prefix=mydir) mydir 是我指定的自定义目录,所有手动安装的软件放在该目录下
```bash
./configure --prefix=mydir --enable-python3interp=dynamic  --with-python3-config-dir=mydir/lib/python3.6/config-3.6m-x86_64-linux-gnu
```

接着
```bash
make
make install
```
安装完成，可以使用 vim --version ，发现版本是8.1，找到vim选项并且显示 **+python3/dyn**
说明安装成功
注意 +python3 后面必须显示 dyn，因为有一些选项需要该设置支持. 安装完成后,在vim 中我们可以用`:py3 print("hello")` 来看看是不是可以执行 python

### 出现的问题以及解决办法
#### 1.安装vim后没有将可执行文件安装到指定目录下
这个问题的原因是什么暂时还不知道，只能详细读懂 makefile 才能知道原因了，但是如果在执行make 和make install都顺利进行的话，即此时我们已经成功了编译了vim，那么就一定会在某个位置存放了vim的可执行文件。
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
进入vim源码压缩包解压后的src目录下,修改if\_py\_both.h 文件
```cpp
// Disabled, because this causes a crash in test86
// OutputType.tp_base = &PyFile_Type;

修改为
// Disabled, because this causes a crash in test86
OutputType.tp_base = &PyFile_Type;
```
很显然,注释都让我们不要这样改了,这是作者的建议,我并没有那么做,只是修改 vim 编译时的选项

#### 安装 git
如果我们使用 vim-plug 插件,它会使用 git 把插件安装到 ~/.vim/plugged 目录下,要在启动 vim 时也启动 vim-plug 安装的插件,需要我们安装Git,否则会使用不正常
