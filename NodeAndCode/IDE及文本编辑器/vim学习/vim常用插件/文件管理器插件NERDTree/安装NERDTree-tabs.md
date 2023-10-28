### 建议安装的原因
NERDTree 中在开启多个tab(标签页) 时,是不共享 NERDTree 目录树的,即当打开新的 tab 时,窗口里面没有 NERDTree,需要我们输入 :NERDTree 重新打开,且打开的 NERDTree 目录树和其它 tab 下的 NERDTree 目录树并不是同一个,这样会很麻烦,显然我们希望像IDE 那样即便打开新的tab ,也能够共享同一个 NERDTree 窗口,且在开启新的 tab 时 NERDTree 应该已经被打开 vim-nerdtree-tabs 就是解决这个问题

### 安装 NERDTree-tabs
我们可以在github 上搜索到这个插件:  
官网:  https://github.com/jistr/vim-nerdtree-tabs  
官网提供 pathogen, vundle 等多种安装方式   
注意,如果使用 Vundle 安装,不建议使用官网的 Bundle 命令,而建议使用最新的 Plugin 命令(这是最新Vundle建议的做法),而是在 ~/.vimrc 中找到 Vundle 配置的地方,添加
```bash
Plugin 'jistr/vim-nerdtree-tabs'
```  
然后打开 vim,输入 :PluginInstall  
找到 Plugin 'jistr/vim-nerdtree-tabs'  并按回车

### 配置 ~/.vimrc  
要启动 vim-nerdtree-tabs ,只需在 .vimrc 添加如下一行  
```bash
let g:nerdtree_tabs_open_on_console_startup=1   
```
这样就可以在终端启动vim 时,默认以当前启动vim 的路径为根目录开启NERDTree,不用再手动输入 :NERDTree 开启,同时,当打开多个 tabs 时,默认在开启新的 tab(标签页) 时会启动 NERDTree,同时同一个 vim 进程中,不同的 tab 共享同一个 NERDTree 目录树,即移动到 NERDTree 窗口时,光标位置,包括 NERDTree 窗口当前展开了哪些文件夹,在不同 tabs 中是一样的
