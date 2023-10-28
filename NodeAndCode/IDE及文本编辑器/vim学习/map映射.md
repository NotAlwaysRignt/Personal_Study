`map`关键字可以帮助我们建立按键的映射,vim有多种模式,如normal模式,insert模式,visual模式,vim允许我们定制在指定模式下的映射
[TOC]
### 不同模式下映射
`imap jk <Esc>`表示在插入模式下连续按下 jk,则相当于按下了Esc键,不同的前缀表示不同的模式,这里不多举例

### 针对不同文件类型使用不同映射
同一个按键可以对不同文件类型做不同的映射,这里的文件类型并不是文件的后缀,而是`filetype`变量的值,可通过`:set filetype`来查看
比如下面的例子,同样按 F3,对于C++类型文件,映射到输入`:hellocpp`,而对于js文件,则映射为`:hellojavascript`
```bash
autocmd FileType cpp nnoremap <F3> :hellocpp
autocmd FileType javascript nnoremap <F3> :hellojavascript
```

### 非递归映射
`nnoremap / /\c\<`是对`/`键的映射,注意nnore,表示在nomal模式下非递归(nore),如果没有nore,那么我们按下`/`,它又映射了一次`/`,接着又触发了一次`/`的映射,无限递归下去,这就不是我们想要的效果了

### 映射时的选项
使用映射时,还支持一些特殊选项,如`<silent> <buffer> <SID>`等,举例:

```bash
xnoremap <silent> <buffer> U  :call <SID>status_update()<cr>
```
x表示可视模式,(v表示选择与可视模式,注意区别,选择模式下可以直接对字符进行输入替换,按backspce删除,而可是模式则是使用normal下指令操作选中字符串,两者不同)
* `<silent>`表示安静模式,即命令`:call status_update()<cr>`不会显示在你的窗口里,我们可以用个更简单的例子
`nnoremap h /helloworld<cr>`,当按下h时,会看见窗口弹出了`/helloworld<cr>`,如果使用`nnoremap <silent> h /helloworld<cr>`,则不会有这种情况
* `<buffer>` 表示只在当前缓冲区有效,也就是说,如果在当前缓冲区使用了map,那么换一个缓冲区这个映射不会生效
* `<SID>` 表示调用的是局部变量,比如上面的`status_update`是一个脚本函数`s:status_update`,如果不用`<SID>`我们是没办法调用它的,只能调用全局函数`g:status_update`
