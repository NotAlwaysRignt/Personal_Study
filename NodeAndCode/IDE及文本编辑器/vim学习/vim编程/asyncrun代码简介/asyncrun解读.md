### AsyncRun 简介
AsyncRun 利用了 vim 的内置异步接口`vim_start`,支持异步运行 shell 命令并输出到quickfix窗口,且不会阻塞当前vim的运行,下面简单介绍阅读代码的思路
AsyncRun 是纯 vim-script 脚本实现的,通过一个 vim 文件实现,非常轻巧,只要 vim 的版本支持异步就可以很方便地使用它

### AsyncRun 定义
`asyncrun.vim`中核心jioiu的两个命令就是`AsyncRun` 和 `AsyncStop` 
```shell
command! -bang -nargs=+ -range=0 -complete=file AsyncRun 
	\ call asyncrun#run('<bang>', '', <q-args>, <count>, <line1>, <line2>)
```
上面是对 AsyncRun 的定义,使用 AsyncRun 时相当于调用了函数`asyncrun#run`
-complete=file 表示使用 AsyncRun 时我们可以用 tab 键对文件名进行补全,如`:AsyncRun ~/.vi + tab` 会补全`AsyncRun ~/.vim`

上面定义在`<>`我们可以用 help 命令查看
* `<q-args>` 表示如果参数的前两个字符为 q- 开头,则它会被赋值,否则赋值空string
* `<count>` 指定 -count 或 -range 属性时被赋值,根据-range=0,可知默认被赋值为0
* `<line1>` 命令的第一行
* `<line2>` 命令的最后一行

看看函数`async#run`的定义
```shell
function! asyncrun#run(bang, opts, args, ...)
```
bang变量只有在使用`AsyncRun!`(加感叹号)时才会被赋值,opts变量从命令来看被置空,args命令是跟在 AsyncRun 后的命令,比如`AsyncRun find -name hello.vim`,那么 a:args 参数值就为`find -name hello.vim`
可变参数 a:000 即`[<count>,<line1>,<line2>]`

我们可以知道最关键的命令被赋值到了 args 中了,因此主要关注 a:args 这个命令就可以大概分析其流程
```shell
	let [l:command, l:opts] = s:ExtractOpt(s:StringStrip(a:args))
    " 如果调用 AsyncRun -raw -cwd=/home/usr/ find -name hello.vim,则ExtractOpt 返回
    "['find -name hello.vim', {'cwd': '/home/usr/', 'save': '', 'program': '', 'mode': '', 'post': '', 'text': '', 'raw': '1','auto': ''}]
```
具体运行见代码,这里暂不深入
