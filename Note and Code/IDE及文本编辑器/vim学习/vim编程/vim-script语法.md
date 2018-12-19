vimscript的中文教程较少,这里推荐不错的参考网站:
http://learnvimscriptthehardway.onefloweroneworld.com/
上面是博客网站,它的内容在github 中也可以找到
https://github.com/isayme/learnvimscriptthehardway-cn

### help 是个好命令
关于 vimscript 的语法,因为内容庞杂,且中文资料并不多,学习起来,比较困难,但是 vim 内置的文档非常齐全,对于我们在阅读学习一些插件是发现不懂的语法,在命令模式中,输入`:help` 即可查找,比如我对自定义命令 command 关键字及相关的选项并不了解,可以输入 `:help command`,使用help 时,按 tab 还可以补全你要找的关键字,help 的搜索非常强大,有模糊匹配的功能,只要是个关键字,我们就可以用help 尝试
比如你要查一个函数index的用法,可以用`:help index(` 这样查到的是函数,如果`:help index`查到的是index这个命令,并不相同
### 练习,测试
写 demo 练习是非常重要的学习方法,对于简单的函数,我们用help 查看功能后,可以用`:echo` 来查看输出,但是复杂点的,比如要自定义函数之类的,那就要自己写脚本了,为了测试验证,我们可以简单地写一个vim脚本,以.vim结尾, 然后启动vim ,用`:source XXX.vim`去加载运行它,`:source` 也可以缩写`:so`,vim 脚本的查找路径默认是启动 vim 时所在的路径,如果要启动特定路径目录下的 vim 文件,可以在vim文件前加路径,如 `so ~/test/myplug.vim`

### 调试输出
编写vim-script时,`echo`可以使我们马上看到打印输出,但一旦脚本运行完毕,输出就会小时,使用 `echom`同样可以打印输出, 同时会保存下来,我们可以用`:messgage`查看

### 单引号和双引号
vimscript 的单引号和双引号是有区别的,其区别在于对于反斜线,单引号包裹的是纯字符,不会进行转义,而双引号会,因此单引号不能表示换行符'\n',因为'\'不表示转义,但双引号"\n"则表示换行,如果要表示斜杠,则要用"\\"

### set
set 相当于 let xxx = 1
`set number` = `let number = 1`会显示行号
### 变量
`g:varname`	变量为全局变量
`s:varname`	变量的范围为当前的脚本文件
`w:varname`	变量的范围为当前的编辑器窗口
`t:varname`	变量的范围为当前的编辑器选项卡
`b:varname`	变量的范围为当前的编辑器缓冲区
`l:varname`	变量的范围为当前的函数
`a:varname`	变量是当前函数的一个参数
`v:varname`	变量是 Vim 的预定义变量
`&`  表示引用,即引用已经存在的变量,而不是重新创建一个
`@`是寄存器变量,比如我们通常使用y进行复制,默认存储在未命名寄存器"中,用`@"`表示,可以用`echo @"`来看里面的内容

### 数据结构
列表和字典两种数据结构,语法上和 python 是很相似的,可类比
#### 列表
`[]`是列表
* 可以嵌套,如`['foo', [3, 'bar']]`
* 列表取索引`:echo [0, [1, 2]][1]`
* 列表可切割,`:echo ['a', 'b', 'c', 'd', 'e'][0:2]`,Vim显示`['a','b','c']`(第1,2,3个元素)。
* 越过列表索引上界也是安全的:`:echo ['a', 'b', 'c', 'd', 'e'][0:100000]`,显示`['a', 'b', 'c', 'd', 'e']`
* 可以用函数len获取列表长度`:echo len([1,2,3])`
#### 字典
`{}`是字典,在字典}的前面有个逗号是允许的,这样也可以避免在多行时出现语法错误
* 取索引`:echo {'a': 1, 100: 'foo',}['a']`,当然也支持 Js 风格的点号取索引`:echo {'a': 1, 100: 'foo',}.a`
* 移除项
```bash
#let foo = {'a': 100, 'b': 200}
:let test = remove(foo, 'a')
:unlet foo.b
:echo foo  # 显示{}
:echo test # 显示 100
```

### 函数
前缀`a:`表示这个是函数的参数,比如我们有个函数`function hello(arg)`,要在函数体里使用这个`arg`参数,必须使用`a:arg`来访问,否则访问的是全局的arg变量,而不会访问到参数
`...`表示可变参数,`function hello(...)`来表示,`a:000`表示这个参数的列表,我们可以用 echo 来查看, `a:0`是参数的个数,`a:1`是第一个参数,`a:2`是第二个参数,以此类推

### 自动命令
所谓自动命令,就是在某些特定事件(比如打开vim,打开某种类型文件)发生时触发命令的自动执行
使用`:help autocmd`可以看到定义自动命令的方法,在编写插件时,我们希望vim在某些事件,比如启动时,或打开窗口时触发,这个时候定义自动命令就很有必要
```bash
:au[tocmd] [group] {event} {pat} [nested] {cmd}
                        Add {cmd} to the list of commands that Vim will
                        execute automatically on {event} for a file matching
                        {pat} autocmd-patterns.
                        Note: A quote character is seen as argument to the
                        :autocmd and won't start a comment.
                        Vim always adds the {cmd} after existing autocommands,
                        so that the autocommands execute in the order in which
                        they were given.  See autocmd-nested for [nested].
```

#### 命令组
命令组是常用的概念,使用`:help augroup`可以查看
```bash
:aug[roup] {name}               Define the autocmd group name for the
                                following ":autocmd" commands.  The name "end"
                                or "END" selects the default group.
                                To avoid confusion, the name should be
                                different from existing {event} names, as this
                                most likely will not do what you intended.
```

### 自定定义 command
`:help command`可以查看 vim 的文档解释
```bash
:com[mand][!] [{attr}...] {cmd} {rep}
                        Define a user command.  The name of the command is
                        {cmd} and its replacement text is {rep}.  The command's
                        attributes (see below) are {attr}.  If the command
                        already exists, an error is reported, unless a ! is
                        specified, in which case the command is redefined.
```
关于attr这里讲解一下,主要是描述这个命令是否需要参数,以及需要参数的情况下需要多少个参数
```
       -nargs=0    No arguments are allowed (the default)
        -nargs=1    Exactly one argument is required, it includes spaces 
        -nargs=*    Any number of arguments are allowed (0, 1, or many),
                    separated by white space   
        -nargs=?    0 or 1 arguments are allowed
        -nargs=+    Arguments must be supplied, but any number are allowed
```
以 vim-plug 插件为例,看看它是如何使用 command 这个命令的
```bash
  "-complete=custom,{func} custom completion, defined via {func}
  "<f-args> ("function args").  This splits the command arguments at spaces and tabs, quotes each argument individually, and the <f-args> sequence is replaced by the comma-separated list of quoted arguments.
  "<bang>就是一个转义符,等价于!,是为了避免语法冲突所以才这样使用,对于call s:install(<bang>0, [<f-args>]),如果命领使用了!,>如PlugInstall!,则<bang>0 传递值为1,若没有用!,则传递值为0
  command! -nargs=* -bar -bang -complete=customlist,s:names PlugInstall call s:install(<bang>0, [<f-args>])
```
使用`:help :command`可以查看选项的含义, 这里讲下`<bang>`,`<bang>`就是个转义符号,相当于 !,但直接用!与 vim的语法冲突了,因此要转义,另外`<bang>0`的含义见注释,`s:names`是一个自定义的函数,用于补全

### 比较
`help ==#`可以查看比较,vim可以直接进行字符串比较,比如`echo  'plugin'=='plugin'`, ==默认是考虑大小写的,如果要忽略大小写,后面加?,即==?,>=? .事实上==是否忽略大小写是可以系统设置的,因此,如果要显示指定考虑大小写,可以加#,如==#,<=#

### 事件
当我们定义自动命令时,常常可以定义触发它的场景,比如
* VimEnter,当进入vim时,定义 `au VimEnter * call s:init()`,则在进入vim时会调用函数 s:init()
* VimLeave ,离开vim时
* InsertLeave ,离开insert 模式
* TextChanged,在 normal 模式下修改文本
* TextChangedI,在insert模式下修改文本
更多用help查看vim文档
还有一个常用的变量是`vim_starting`,这个是vim内置的变量,当vim启动时(这个时候我们还没有进入vim界面),`has('vim_starting')`为true,当我们进入vim界面进行操作时,`has('vim_starting')`为false,我们常用来进行启动vim时的初始化操作
