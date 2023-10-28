本文将介绍 vim-plug 的加载插件的执行流程以及使用方法

我们在`.vimrc`上只要使用三个 vim-plug 暴露的接口
* plug#begin,如`call plug#begin('~/.vim/plugged')`
* Plug命令,如`Plug 'scrooloose/nerdtree'`
* plug#end,`call plug#end()`
因此先讲解这三个函数,具体的源码注释放在另一个文件里,这里讲解其主要流程
###  plug#begin
plug#begin 是使用 vim-plug 时,必须在`.vimrc`最先调用的函数,我们可以在`plug.vim`中查看其定义`function! plug#begin(...)`,这个函数主要做了以下的事情
* 给全局变量`g:plug_home`赋初值,`g:plug_home`我们可以使用`:echo g:plug_home`查看最终结果,通常设置为`~/.vim/plugged`,我们使用git安装的拓展插件都会放在这个目录下,另外它给`g:plugs,g:plugs_order`分配了空间,但此时还没有赋值
* 定义 Plug,PlugInstall 等命令,我们可以看到其中一句`call s:define_commands()`,进入这个函数,可以看到
```bash
function! s:define_commands()                                                                                             
  command! -nargs=+ -bar Plug call plug#(<args>) 
  if !executable('git')
    return s:err('`git` executable not found. Most commands will not be available. To suppress this message, prepend `silent!` to `call plug#begin(...)`.')
  endif
  command! -nargs=* -bar -bang -complete=customlist,s:names PlugInstall call s:install(<bang>0, [<f-args>])
  command! -nargs=* -bar -bang -complete=customlist,s:names PlugUpdate  call s:update(<bang>0, [<f-args>])
  ".......
```
详细含义可以看另一份有注释的`plug.vim`
### Plug 命令
Plug的用法可以直接查看`plug.vim`的代码,里面开头的注释就详细介绍了其用法
Plug命令在 plug#begin 中定义
```bash
command! -nargs=+ -bar Plug call plug#(<args>)
```
所以当我们在`.vimrc`中写入`Plug 'scrooloose/nerdtree'`时,相当于调用函数`plug#('scrooloose/nerdtree')`
可以找到这个函数`function! plug#(repo, ...)`
比如我们在配置文件中写了如`Plug 'rdnetto/YCM-Generator'`的形式,那么 `{ 'branch': 'stable' }` ,'rdnetto/YCM-Generator'会传递给 repo,而后面的{ 'branch':'stable'}则传递给了 ...作为可变参数
使用Plug'XX/XX'时,函数会解析插件`XX/XX`信息,比如该插件是git仓库还是本地的,如果是git仓库的要格式化成对应的远程url,以及解析版本信息,最后这些信息会用于初始化g:plugs和 g:plugs_order 这两个变量,我们可以在vim里用`echo g:plugs`来看看它们长什么样子, 这里只是初始化变量,但不涉及插件的加载

### plug#end()
通过 plug#begin 设置了插件的安装路径, Plug 设置了插件的查找路径, plug#end 函数会根据 `g:plug_home` `g:plug_order` 和 `g:plugs`来查找并加载这些插件
首先它会根据这些变量来补充运行时路径 &rtp,我们可以使用`echo &rtp`来看最后结果,假设`g:plug_home`为`~/.vim/plugged`而我们设置了`Plug 'scrooloose/nerdtree'`,那么`~/.vim/plugged/nerdtree`将被加入运行时路径
然后它找到插件目录,使用vim内置函数 globpath 查找所有符合`'plugin/**/*.vim'`和`'after/plugin/**/*.vim'`模式的vim文件,并用souce命令加载它们,注意`'plugin/**/*.vim'`这种模式匹配到的不仅是`plugin`目录再下一级目录下的vim文件,也会匹配到plugin目录下的vim文件,及`plugin/test.vim`和`plugin/dir/test.vim`都会被匹配到

### 使用 vim-plug 定制自己的插件
通过上面的讲解,我们知道了vim-plug 是如何加载插件了,这样我们也就能写自己的插件
我们要组织插件目录结构很简单,对于启动时就要加载的插件,放在{project}/plugin目录下,{project}表示我们创建一个新插件的根目录,如果我们要放在github上,则{project}表示仓库的根目录,我们可以参考其它插件的格式,发现它们也是这样组织的.
对于运行时要加载的插件(即当我们调用命令时才会去加载的插件),放在{project}/autoload 目录下
举例,我们自己建立一个test插件,放在`~/.vim/plugged`目录下
```bash
#在~/.vim/plugged/test下执行tree命令看目录结构
.
├── autoload
│   └── test.vim
└── plugin
    └── test.vim
```
`plugin/test.vim`内容
```bash
echom "this is plugin test.vim"
```

`autoload/test.vim`内容
```bash
echom "autoload"
function g:test#Test()
    echom "the autoload has been loaded"
endfunction
```
我们在 .vimrc 中的配置
```bash
call plug#begin('~/.vim/plugged')
"其它插件省略
Plug '/home/ubuntu/.vim/plugged/test'
call plug#end()
```
这里我使用的是本地插件加载的方式,注意 `Plug '/home/ubuntu/.vim/plugged/test'` 也可以相对用户路径`~/.vim/plugged/test`
根据上面的讲解,/home/ubuntu/.vim/plugged/test会被加载进入 &rtp 变量,我们可以用`:echo &rtp`来查看
我们使用vim命令时
```bash
this is plugin test.vim
Press ENTER or type command to continue
```
可见,加载成功,因为 vim 启动时会先去加载 plugin 目录下的vim文件
进入 vim 后,输入`::call g:test#Test()`,打印如下:
```bash
autoload
the autoload has been loaded
Press ENTER or type command to continue
```
注意`autoload`这个字符串在这时才显示,可见 autoload 目录下的vim文件在vim刚启动时并没有被加载执行,只有等到需要调用相应的脚本才加载,这也可以减少vim的启动时间
