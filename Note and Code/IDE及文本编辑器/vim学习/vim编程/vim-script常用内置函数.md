参考文章: https://blog.csdn.net/lihaicg/article/details/8364349  

如果我们不知道一个函数的用法,可以使用help查看,vim内置的文档说明得非常详细,需要注意的是,如果有的命令和函数重名,比如`filter`既可以是vim的一个命令,也可以是一个内置函数,使用`:help filter`查到的是内置命令的内容,为了查到内置函数,请使用`:help filter(`或`:help filter`通常半个括号就可以了

下面会对一些常用的函数做解析,具体用法可以用 help 查看
### 运行命令
`exec`是 vim-script 的关键字,它接收一个字符串参数,相当于进入 command 模式并运行它,如`exec "wa"` 与 键入`:wa`并按回车, 效果是一样的
有了 `exec` 这个命令,我们可以将自己编写的插件和别的插件结合起来,如
```shell
exec "Async Run python3 $(VIM_FILEPATH)"
exec "call FcuntionOfOtherPlugin()"
```

### 处理单行
#### line ("expr")
返回值是一个整数。这个函数功能时返回一个行号。如果你后面的参数expr是"."的话，则返回当前光标所在的位置。如果extr是"$"则返回当前缓冲区最后一行的行号。如果extr的值是一个标签名,则返回你对应的标签所在的行号。
例如：call line(".") 这条语句执行的结果是得到当前光标所在的行号。
#### setline ({lnum}, {text} )
这个函数就是将第lnum行的内容替换成text中的内容。如果这个lunm行是最后一行的下面一行，那么将新建一行，然后将text的内容存放到这一行。如果不是最后一行的下一行，则要满足的条件是行号为lnum的行必须是存在的。
例如:`call setline(1,'hello world')`
这句话就是文件的开头添加上面的一行，主要是用于C/CPP文件注释的。

#### getline(({lnum},{end})
返回值是一个字符串。当end参数存在的时候，函数返回行号为lnum的内容，否则函数将返回行lnum到行end的所有内容（包括第end行）。
例如：
let  lines=getline(2,5)
call append(6,lines)
上面两句话表示取得2-5行的内容，然后从第六行的下一行开始添加，保持原来的格式不变。

### 文件相关处理
#### fnamemodify({fname}, {mods})
意为 file name modify,会根据第二个参数mod去修改第一个字符串 fname,返回修改后的字符串,
关于第二个 mod,可以用`help filename-modifiers`查看,常用的有: 
* `:t`,tail of the filename,文件路径中去最后一个文件名,如 `fnamemodify('/home/user/hello.cpp',':t')`得到 hello.cpp
* `:h`,Head of the file name,去掉最后一个文件名,获取剩下的路径,如`fnamemodify('/home/user/hello.cpp',':h')`获得`/home/user`
* `:s?pat?sub?` 类似`:s/pat/sub`字符串替换的功能
* `fnamemodify`中的mod选项,可以组合使用,像`fnamemodify('/home/user/hello.cpp',':h:t')`,得到`user`,注意顺序是会影响的,这里先取:h再取:t

#### glob 和 globpath
`globpath({path}, {expr} [, {nosuf} [, {list} [, {alllinks}]]])`相当于对{path}下的所有路径执行glob(),简单地说就是会去解析path,以及通配符,在系统中找到符合的文件名,并和path组成字符串返回,如果结果有多个,则每个结果以换行符分隔
举例:`globpath('/home/user/mydir','*.vim')`.假设mydir目录下,存放了几个vim文件`hello.vim`,`world.vim`,则结果返回一个字符串
```bash
/home/user/mydir/hello.vim
/home/user/mydir/world.vim
```
第一个参数path可以有多个路径,以逗号分隔,如`:echo globpath(&rtp, "syntax/c.vim")` 会到所有 rtp 下查找
使用`globpath('/home/user/mydir/','plugin/**/*.vim')`时,匹配到的不仅是`plugin`目录再下一级目录下的vim文件,也会匹配到plugin目录下的vim文件,及`plugin/test.vim`和`plugin/dir/test.vim`都会被匹配到
更多用法看文档
