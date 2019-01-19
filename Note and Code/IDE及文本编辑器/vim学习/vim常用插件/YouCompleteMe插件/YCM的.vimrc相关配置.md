[TOC]
YCM 的可配置项非常多,最权威的自然是官网,我们有必要认真阅读官网的配置项,根据需要配置,这里讲解常用配置
参考文章 [YouCompleteMe 中容易忽略的配置
](https://zhuanlan.zhihu.com/p/33046090)
### 非常有必要的配置

#### 输入字母时自动补全
默认的 YCM 配置并不完美,比如它只在输入'.','->','::' 才会进行语义分析,比如我们输入 printf,它是不会提示的,除非我们以前输入过,但输入过才会提示显然也不能让我们满意,解决这个问题,可以添加以下配置
```bash
let g:ycm_semantic_triggers =  {
            \ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
            \ 'cs,lua,javascript': ['re!\w{2}'],
            \ }
```
`['re!\w{2}']` 的意思是,使用正则表达式,当我们输入两个连续的字母时,就会启动补全提示

#### 与 Ultisnips 共存
YCM 占用了快捷键`<tab>`和`<S-tab>`,分别表示指向补全菜单的下一项和上一项.本人习惯用 tab 做补全操作,因此对此冲突.我想把`<tab>`作为 YCM 补全的触发,而`<S-tab>`作为 Ultisnips 的代码块生成触发
首先明确冲突的原因:YCM 使用了这个快捷键,作为菜单移动
> Note that one of the defaults is <S-TAB> which means Shift-TAB. That mapping will probably only work in GUI Vim (Gvim or MacVim) and not in plain console Vim because the terminal usually does not forward modifier key combinations to Vim.
Default: ['<S-TAB>', '<Up>']

即默认`let g:ycm_key_list_previous_completion = ['<S-TAB>', '<Up>']`
因此,只要设置菜单移动不要使用该快捷键,那么就可以把`<S-tab>`交给 Ultisnips 了
本人设置
`let g:ycm_key_list_previous_completion = ['<Up>']`
`let g:UltiSnipsExpandTrigger="<S-tab>"`
这样就把问题解决了

#### 关闭预览窗口
另外还有一个很不喜欢的功能,就是补全时还会弹出一些预览窗口,说明函数功能,这些窗口很多时候不会有有用的信息,禁用掉它可以设置  
```bash  
set completeopt=menu,menuone
let g:ycm_add_preview_to_completeopt = 0
```  

#### 跳转到定义或声明
YCM 自带了跳转到定义或声明的功能,使用方法,输入:
:YcmCompleter GoToDefinition (跳转到定义)
:YcmCompleter GoToDeclaration (跳转到声明)ycm_complete_in_strings
YcmCompleter GoToDefinitionElseDeclaration  (跳转到定义,不能跳转则跳转到声明)  
上面的跳转除了可以进入函数或变量的定义或声明以外,YCM 也支持文件间的跳转,当光标在 include 里的文件名上时执行跳转就可进入文件  
虽然可以在输入命令时用 Tab 补全,但是还是不方便.所以建议使用快捷键映射
  
`map gd :YcmCompleter GoToDefinitionElseDeclaration<CR>`;  
即把 `:YcmCompleter GoToDefinitionElseDeclaration` 映射到快捷键 gd,这样可以很方便地查看函数
当跳转到文件时,如果想跳转回来
ctrl o (跳转回前一个位置)
**YCM的跳转有时候并不令人满意,而且还要映射一些快捷键,建议使用 ctags 的跳转功能**

#### 屏蔽诊断信息
YCM 默认会显示诊断信息,如果不需要,可以使用`let g:ycm_show_diagnostics_ui = 0`关闭
我们可以使用其它插件来做检查

#### `.ycm_extra_conf.py`相关配置
个人建议使用`g:ycm_extra_conf_globlist`配置一个全局的的`.ycm_extra_conf.py`,对于多用户使用一台机器的情况,设置只允许加载本用户目录下的`.ycm_extra_conf.py`

然后使用`g:ycm_confirm_extra_conf = 0`禁止 YCM 询问是否加载`.ycm_extra_conf.py`
关于`.ycm_extra_conf.py`的作用及详细配置见下文说明

如果要针对某个具体项目写,可以使用`rdnetto/YCM-Generator`这个 vim 插件,它会解析 makefile 文件生成 flag,因此需要依赖`makefile`
### 其它配置说明
```bash
" 自动补全配置
set completeopt=longest,menu    "让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
autocmd InsertLeave * if pumvisible() == 0|pclose|endif "离开插入模式后自动关闭预览窗口
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"    "回车即选中当前项
"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"youcompleteme  默认tab  s-tab 和自动补全冲突
"let g:ycm_key_list_select_completion=['<c-n>']
let g:ycm_key_list_select_completion = ['<Down>']
"let g:ycm_key_list_previous_completion=['<c-p>']
let g:ycm_key_list_previous_completion = ['<Up>']
let g:ycm_confirm_extra_conf=0 "关闭加载.ycm_extra_conf.py提示

let g:ycm_collect_identifiers_from_tags_files=1 " 开启 YCM 基于标签引擎
let g:ycm_min_num_of_chars_for_completion=2 " 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0  " 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_seed_identifiers_with_syntax=1    " 语法关键字补全
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>    "force recomile with syntastic
"nnoremap <leader>lo :lopen<CR> "open locationlist
"nnoremap <leader>lc :lclose<CR>    "close locationlist
inoremap <leader><leader> <C-x><C-o>

"在注释输入中也能补全
let g:ycm_complete_in_comments = 1
"在字符串输入中也能补全
let g:ycm_complete_in_strings = 1
"注释和字符串中的文字也会被收入补全
let g:ycm_collect_identifiers_from_comments_and_strings = 0

nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR> " 跳转到定义处
```

### .ycm_extra_conf.py 配置
#### `.ycm_extra_conf.py`的功能
YCM可不会解析`makefile`,它不知道我们的编译 flag 是什么样,自然也不知道我们是不是使用 -I 加载了别处的头文件,为了解决这个问题,YCM要求我们自己编写一个`.ycm_extra_conf.py`,以告诉YCM这些信息,如果YCM 找不到这个文件,那么无法提供语法补全

#### `.ycm_extra_conf.py`的存放位置
`.ycm_extra_conf.py`应该放置在哪? 先看看官网对这个文件的介绍
> CM looks for a .ycm_extra_conf.py file in the directory of the opened file or in any directory above it in the hierarchy (recursively); when the file is found, it is loaded (only once!) as a Python module. YCM calls a Settings method in that module which should provide it with the information necessary to compile the current file. You can also provide a path to a global configuration file with the g:ycm_global_ycm_extra_conf option, which will be used as a fallback.

当我们打开一个文件的时候,YCM首先会在这个文件的目录查找`.ycm_extra_conf.py`文件,如果没找到,就会向上一层目录递归查找
YCM 也允许我们自己在`.vimrc`中设置`g:ycm_global_ycm_extra_conf`变量,让我们提供一个全局默认的`g:ycm_global_ycm_extra_conf`,当YCM 在项目目录中没有找到`.ycm_extra_conf.py`时就会选用这个全局的`.ycm_extra_conf.py`

#### 一个最简单的`.ycm_extra_conf.py`
YCM的官网提供了一个最简单的`.ycm_extra_conf.py`示例,通常情况下,我们只需要提供 flag 信息,YCM 就能够知道编译规则,并为我们提供补全提示.本人认为最重要的是头文件的信息,只要有此信息,就能够知道整个项目的编译关系
官网给出的例子在`C-family Semantic Completion`的`Option 2: Provide the flags manually`处
假设我们编译时候的flag是这样的`g++ -Wall -Wextra -Werror -o FILE.o FILE.cc`,那么
```python
# `.ycm_extra_conf.py`的内容只要这样即可
def Settings( **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-Wall', '-Wextra', '-Werror' ],
  }
```
定义一个`setting`函数,然后返回一个字典,其中字典有一个`flags`的 key,其值就是 flag, YCM 建议我们有必要加上`-x` flag,告诉 YCM 这个项目是什么类型的项目,比如 C语言使用`-x c`,`C++`用`-x c++`

`-I`是常用的功能,使用相对路径时是相对`.ycm_extra_conf.py`的路径,如`-I./include/`会查找与被加载的`.ycm_extra_conf.py`同级的`include`目录

对于复杂的项目,仅仅有flags 信息显然还是不够的,ycmd 目录下提供了一个`.ycm_extra_conf.py`示例文件
注意这个文件只是作为参考,如果只是把它拷贝来用很可能在自己系统上工作不起来,毕竟系统头文件的位置很可能和这个 demo 文件不同.

我们可以自己编写一个全局的`.ycm_extra_conf.py`,并用`.ycm_extra_conf.py`指向它
`YouCompleteMe/third_party/ycmd/examples`下有一个`.ycm_extra_conf.py`,实践证明用`g:ycm_global_ycm_extra_conf`指向它,已经可以满足C++ 标准库的补全了,当然也可能在自己系统上工作不起来,毕竟系统头文件的位置很可能和这个 demo 文件不同,所以还是建议重写一个修改一下,主要是修改`flags`,我们可以模仿`flags`的`-isystem`指向目录根据自己操作系统实际情况修改

如果要针对某个具体项目写,可以使用`rdnetto/YCM-Generator`这个 vim 插件,它的主要功能是解析当前目录中的`Makefile`,根据调用`make`命令时会用到的 flags,生成`.ycm_extra_conf.py`

#### g:ycm_extra_conf_globlist
为了能够能够控制将指定目录下的`.ycm_extra_conf.py`载入,YCM 通过变量`g:ycm_extra_conf_globlist`设置白名单和黑名单
这是一个列表变量,列表里每个成员都是字符串,并允许使用 YCM 规定的通配符
* `*` matches everything
* `?` matches any single character
* `[seq]` matches any character in seq
* `[!seq]` matches any char not in seq

#### g:ycm_confirm_extra_conf
YCM 会在进入文件时去加载`.ycm_extra_conf.py`,但是这可能带来一个安全问题,比如它可能加载到不是自己编写的带有恶意代码的`.ycm_extra_conf.py`(虽然这个可能性很小)
YCM 默认设置`let g:ycm_confirm_extra_conf = 1`,意思是当搜寻到可用的`.ycm_extra_conf.py`时,会要求用户确认一次,如果不想要确认,则可以置为0
为了避免每次都要确认,个人认为还是使用一个全局的`.ycm_extra_conf.py`,然后禁止每次询问
如果机器是多人使用的,那么还是在`g:ycm_extra_conf_globlist`中设置只允许加载home目录下的`.ycm_extra_conf.py`

举例:`let g:ycm_extra_conf_globlist = ['~/dev/*','!~/*']`
`~/dev/*`表示`~/dev/`目录下(递归地)的`.ycm_extra_conf.py`会被查找并被加载
`!~/*`表示home目录下的所有`.ycm_extra_conf.py`都不会被加载
列表有优先级之分,`~/dev/*`的优先级高于`!~/*`.所以结合起来就是,除了`~/dev/`目录下的`.ycm_extra_conf.py`,home目录下所有的`.ycm_extra_conf.py`都不会被加载

