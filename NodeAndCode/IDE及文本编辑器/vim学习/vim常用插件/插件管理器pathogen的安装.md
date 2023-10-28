### 确保有 .vimrc 文件 以及 .vim 目录
vim会自动到～/.vim 下找文件加载
也会自动到～/.vimrc中加载配置
这两个是隐藏文件，如果没有，可以自己创建，创建后vim就会自动到此处找配置

我的ubuntu安装时有vim但没有.vim，于是自己创建了.vim和.vimrc文件
创建后它们会自动被vim加载

### pathogen的安装：

在.vim文件夹下创建autoload文件夹(vim会自动到.vim的autoload中找配置)
下载pathogen.vim   
并拷贝到
autoload文件夹中
于此我们完成了pathogen的安装
之后我们还要**在.vim中创建名为 bundle 的文件夹(pathogen 会自动到bundle下寻找插件)**,以后要下载插件,可以直接到 github 直接把插件 git clone 到 bundle 目录里,非常简单且易管理
### 示例:使用pathogen 安装python-mode
确认已经在.vim中创建了名为 bundle 的文件夹
进入bundle文件夹，打开终端
git clone https://github.com/klen/python-mode.git
于是bundle文件夹下就会有python-mode文件夹
pathogen的管理插件效果就是自动到bundle下寻找插件，
因为bundle文件夹中 不同的插件又有自己的一个文件夹，进入bundle，装了哪些插件
一目了然，也方便管理

在自己创建的.vimrc中添加
" Pathogen load
filetype off

call pathogen#infect()
call pathogen#helptags()

filetype plugin indent on
syntax on
这些都时官网的说明

注意，检查python-mode是否成功启动
要用vim打开一个py文件！！！
如果用vim打开一个txt这些文件是看不见语法高亮这些效果的
如何检查vim是否成功安装了插件？
进入vim
：scriptnames
就可以看见自己安装了哪些插件

