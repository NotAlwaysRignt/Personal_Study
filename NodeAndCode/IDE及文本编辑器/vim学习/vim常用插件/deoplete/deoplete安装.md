deoplete 相较于 YCM, 安装要容易很多, 不用调用 cmake 等进行编译,对于 C 补全,只要安装一个 clang 即可,缺点是速度比 YCM 要慢不少
下文介绍的是如何在 Vim8 上安装 deoplete 并支持 C++ 补全
### deoplete 安装教程
安装方法在[官网](https://github.com/Shougo/deoplete.nvim)已经给出
我们首先要在系统上安装好`clang`,并确保可以运行clang命令,接着下载`deoplete`插件
```bash
if has('nvim')
  Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
else
  Plug 'Shougo/deoplete.nvim'
  Plug 'roxma/nvim-yarp'
  Plug 'roxma/vim-hug-neovim-rpc'
endif
let g:deoplete#enable_at_startup = 1
```
要支持Vim8除了安装`Shougo/deoplete.nvim`还要安装几个配件,以官网为准
另外我们的 Vim 需要内嵌 python3 支持,可以通过`:echo has("python3")`来查看值,返回 1 表示支持
并且 vim 使用的 python 必须已经安装了`pynvim`,可以用`pip`安装或者离线下载,安装成功后,调用`:python3 import neovim`,import成功则说明安装成功

如果deoplete 安装成功,那么输入时会自动弹出补全,不需要按tab键等快捷键,如果没有,尝试以下命令,只有以下命令都成功时,deoplete 才能正常运行
```bash
:python3 import neovim # 不报错
:echo has('pythonx') # 返回1
:echo exepath('python3') # 能够显示python3的执行文件位置
:echo neovim_rpc#serveraddr() # 能显示服务器的IP地址
```
#### 常见错误
要正常使用 deoplete,我们还需要在 vim 调用的 python 版本中安装`pynvim`,如果没有安装,则以上步骤完成后,打开 vim 仍然不能进行补全,我们发现启动vim时下方会报错误`[vim-hug-neovim-rpc] Vim(pythonx):Traceback (most recent call last)`
输入`:message`可以查看完整的报错信息:
```bash
[vim-hug-neovim-rpc] failed executing: pythonx import neovim
[vim-hug-neovim-rpc] Vim(pythonx):Traceback (most recent call last)
```
我们可以输入`:pythonx import neovim`,发现 import 失败
根据官网本人调用 pip3 安装了 pynvim, 并使用 python3 命令,尝试`import neovim`获得了成功,但启动 vim 后仍然失败
后来发现原因是自己的 ubuntu 系统上安装了两套 python3, 一个是系统自带的`python 3.5`,在`/usr/bin`目录下, 一个是自己使用`apt-get`安装的`python3.6`,在`/usr/local/bin`目录下(输入`which python3`可以知道当前调用的python3 路径在哪)
通过在终端输入 python3 后发现调用的是`python 3.6`(非系统自带的版本),
而我们进入vim后,可以在vim中调用python命令,先后输入`:python3 import sys`,`:python3 print(sys.version)`,可以发现显示的是 3.5 而非 3.6 版本,这是因为使用 apt-get 安装 vim8 时, vim8 会默认支持 python3,但默认使用的是系统 python3
而之前自己使用的 pip3 命令也是在`/usr/local/bin`目录下,通过`pip3 --version`也可以看出,这是 python3.6 版本的

综上: vim 使用的是系统上自带的 python3.5 版本,而后面我用 pip3 安装的`pynvim`装到了后面自己安装的 python3.6上了,所以 vim 仍然无法使用安装的`pynvim`

解决办法有两个:
* 将vim的python切换到我们自己安装的python3.6 而不是 系统自带的 python3.5 上
* 为系统自带的 python3.5 也安装上`pynvim`

第一个操作难度有点大,必须先卸载vim,但是暂时没有了解到在使用`apt-get`安装 vim 时如何指定 python 版本,当然可以使用源码安装,缺点是更新版本就不方便
这里介绍采用第二个思路的解决办法,为系统自带的 python3 安装`pynvim`,进入`/usr/bin`目录,发现没有`pip3`这个可执行文件,我们还得为系统的 python3 安装 pip,很简单,命令`sudo apt-get install python3-pip`即可安装
这时我们可以在`/usr/bin`目录下找到 pip3 了,它可以管理系统 python 的包
接着我们`cd /usr/bin`,调用`./pip3 install --user pynvim --upgrade`,安装`pynvim`,注意`./`一定得加上,不然会调用到`/usr/local/bin`目录下的 pip3,而不是系统 python 的 pip3

如果环境没有连接网络,可以到 pypi 处查找 [pynvim](https://pypi.org/project/pynvim/#files) 离线下载安装
打开 deoplete,不再报错,输入时可以补全,问题解决了
