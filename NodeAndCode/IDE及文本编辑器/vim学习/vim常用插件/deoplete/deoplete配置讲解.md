deoplete安装成功后,使用 `let g:deoplete#enable_at_startup = 1`即可启动补全
对于不同语言的支持,我们还需要安装额外的插件,具体见官网的source 部分讲解
这里只介绍其中几种补全

### deoplete 支持 C/C++ 补全
使用 deoplete 补全 C/C++,需要在系统上安装clang 并可以执行该命令.此时即可对C++进行补全
但是为了更好地体验,应该安装`Shougo/deoplete-clangx`插件,安装完毕后
我们可以在根目录(即启动 vim 时所在的目录,nertree目录树的根目录,nertree中可以用cd切换)下创建`.clang`或`.clang_complete`文件,里面可以存放自定义的头文件路径,文件书写很简单,官网提供示例如下:
```bash
-DDEBUG
-include ../config.h
-I../common
-I/usr/include/c++/4.5.3/
-I/usr/include/c++/4.5.3/x86_64-slackware-linux/
```
直接把 flag 写上即可

`deoplete-clangx`还支持指定clang的可执行文件和库的路径,具体参见官网

### Go补全
三个要求:安装 gocode,安装 go 的补全source, 配置gocode的位置
```bash
Plug 'deoplete-plugins/deoplete-go', { 'do': 'make'}
let g:deoplete#sources#go#gocode_binary="YourDir/gocode"
```
### 常用配置
文档是最好的学习助手,可通过`:help deoplete`和`:help deoplete-options`查看
常用配置及注释如下
```bash
call deoplete#custom#option({
\ 'auto_complete':v:true, # 默认为true,这样就不用按快捷键来补全了
\ 'auto_complete_delay': 50, # 单位ms,一般不设为0,避免频繁计算
\ 'smart_case': v:true,
\ 'force_completion_length':2,
\ })

let g:deoplete#enable_at_startup = 1
let g:deoplete#enable_ignore_case = 1
let g:deoplete#enable_smart_case = 1
let g:deoplete#enable_camel_case = 1
let g:deoplete#file#enable_buffer_path = 1

set completeopt=menuone # completeopt是vim的配置选项,可以用help查看,我们令其=menuone即可
# 注意不要选择preview,这样在函数补全时会弹 preview窗口体验不好
# 也不要使用 noinsert,这样不会默认框选到第一个补全选项,否则要选中第一个补全选项时要按下Enter才会补全而不是按tab(按tab选中了下一个),体验并不好,自己可以试试
```
### 调试
在Vim8中,deoplete 同样是使用server-client 模式,启动vim时后台启动一个server进程,而我们在输入时作为client把字符串发送到 server,经过server解析后把补全结果返回
[vim-hug-neovim-rpc](https://github.com/roxma/vim-hug-neovim-rpc)的README.md中给出了如何查看 server 的信息,便于我们在补全出问题时进行调试
调用`:echo neovim_rpc#serveraddr()`可以查看 server 地址
另外在`.vimrc`中添加如下配置可以看server的日志以调试
```bash
let $NVIM_PYTHON_LOG_FILE="/tmp/nvim_log"
let $NVIM_PYTHON_LOG_LEVEL="DEBUG"
```

另外我们还可以打开客户端的日志
```bash
"call deoplete#custom#option('profile',v:true)
"call deoplete#enable_logging('DEBUG','/tmp/log' )
```

### 注意事项
`set completeopt=menuone`中`completeopt`是 vim 的配置选项,可以用 help 查看具体值的含义,我们令其`= menuone` 即可`set completeopt=menuone`
注意不要选择preview,这样在函数补全时会弹 preview 窗口体验并不好(YCM也是,不要preview),如果我们只是想关闭`preview`,可以使用`set completeopt-=preview`
也不要使用`noinsert`,这样不会默认框选到第一个补全选项,否则要选中第一个补全选项时要按下Enter才会补全而不是按tab(按tab选中了下一个),体验并不好,自己可以试试

如果我们想用tag补全,则可以使用
```bash
"call deoplete#custom#option('sources', {
"\'_': ['buffer'], 
"\ 'cpp': ['buffer', '.tags'],
"\})
```
但是这样的话就不会用`clang`补全了,记得得把`sources`给加上,具体本人还没有研究,因为clang通常已经足够了
