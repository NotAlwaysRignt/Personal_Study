[TOC]
### .ycm_extra_conf.py 配置
#### `.ycm_extra_conf.py`的功能
YCM可不会解析`makefile`,它不知道我们的编译 flag 是什么样,自然也不知道我们是不是使用 -I 加载了别处的头文件,为了解决这个问题,YCM要求我们自己编写一个`.ycm_extra_conf.py`,以告诉YCM这些信息,如果YCM 找不到这个文件,那么无法提供语法补全
具体的配置使用请见本人github的另一篇关于`.ycm_extra_conf.py`的说明

#### `.ycm_extra_conf.py`的存放位置
`.ycm_extra_conf.py`应该放置在哪? 先看看官网对这个文件的介绍
> CM looks for a .ycm_extra_conf.py file in the directory of the opened file or in any directory above it in the hierarchy (recursively); when the file is found, it is loaded (only once!) as a Python module. YCM calls a Settings method in that module which should provide it with the information necessary to compile the current file. You can also provide a path to a global configuration file with the g:ycm_global_ycm_extra_conf option, which will be used as a fallback.

当我们打开一个文件的时候,YCM首先会在这个文件的目录查找`.ycm_extra_conf.py`文件,如果没找到,就会向上一层目录递归查找
YCM 也允许我们自己在`.vimrc`中设置`g:ycm_global_ycm_extra_conf`变量,让我们提供一个全局默认的`g:ycm_global_ycm_extra_conf`,当YCM 在项目目录中没有找到`.ycm_extra_conf.py`时就会选用这个全局的`.ycm_extra_conf.py`

#### 一个最简单的`.ycm_extra_conf.py`
YCM 的官网提供了一个最简单的`.ycm_extra_conf.py`示例,通常情况下,我们只需要提供 flag 信息,YCM 就能够知道编译规则,并为我们提供补全提示.本人认为最重要的是头文件的信息,只要有此信息,就能够知道整个项目的编译关系
官网给出的例子在`C-family Semantic Completion`的`Option 2: Provide the flags manually`处
假设我们编译时候的flag是这样的`g++ -Wall -Wextra -Werror -o FILE.o FILE.cc`,那么
```python
# `.ycm_extra_conf.py`的内容只要这样即可
def Settings( **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-Wall', '-Wextra', '-Werror' ],
  }
```

这里定义一个`setting`函数,然后返回一个字典,其中字典有一个`flags`的 key,其值就是 flag, YCM 建议我们有必要加上`-x` flag,告诉 YCM 这个项目是什么类型的项目,比如 C语言使用`-x c`,`C++`用`-x c++`

`-I`是常用的功能,使用相对路径时是相对`.ycm_extra_conf.py`的路径,如`-I./include/`会查找与被加载的`.ycm_extra_conf.py`同级的`include`目录

对于复杂的项目,仅仅有flags 信息显然还是不够的,`YouCompleteMe/third_party/ycmd/examples` 目录下提供了一个`.ycm_extra_conf.py`示例文件
注意这个文件主要用作参考,实践证明用`g:ycm_global_ycm_extra_conf`指向它,已经可以满足C++ 标准库的补全了,当然也可能在自己系统上工作不起来,毕竟系统头文件的位置很可能和这个 demo 文件不同,所以还是建议重写一个修改一下,主要是修改`flags`,我们可以模仿`flags`的`-isystem`指向目录根据自己操作系统实际情况修改
**注意这是一个 C 语言家族的 `.ycm_extra_conf.py`,其它语言写法类似,但有所不同,见官网**

如果要对项目快速生成`.ycm_extra_conf.py`,可以**使用`rdnetto/YCM-Generator`这个Vim插件**,它的主要功能是解析当前目录中的`Makefile`,根据调用`make`命令时会用到的 flags,生成`.ycm_extra_conf.py`

####`g:ycm_global_ycm_extra_conf`
设置全局默认的`.ycm_extra_conf.py`位置,建议一定要设置一个,因为 YCM 必须找到一个`.ycm_extra_conf.py`,否则即使是C++ 标准库的函数补全也做不了,本人经常要写一些小程序,因此有必要一个默认的配置
可以参考下面给的示例来改
**注意,指定路径时,要用绝对路径,对于home目录不能用`~`**

#### g:ycm_confirm_extra_conf
YCM 会在进入文件时去加载`.ycm_extra_conf.py`,但是这可能带来一个安全问题,比如它可能加载到不是自己编写的带有恶意代码的`.ycm_extra_conf.py`(虽然这个可能性很小)
YCM 默认设置`let g:ycm_confirm_extra_conf = 1`,意思是当搜寻到可用的`.ycm_extra_conf.py`时,会要求用户确认一次,如果不想要确认,则可以置为0

#### g:ycm_extra_conf_globlist
为了能够能够控制将指定目录下的`.ycm_extra_conf.py`载入,YCM 通过变量`g:ycm_extra_conf_globlist`设置白名单和黑名单
这是一个列表变量,列表里每个成员都是字符串,并允许使用YCM规定的通配符
* `*` matches everything
* `?` matches any single character
* `[seq]` matches any character in seq
* `[!seq]` matches any char not in seq

举例:`let g:ycm_extra_conf_globlist = ['~/dev/*','!~/*']`
`~/dev/*`表示`~/dev/`目录下(递归地)的`.ycm_extra_conf.py`会被查找并被加载
`!~/*`表示home目录下的所有`.ycm_extra_conf.py`都不会被加载
列表有优先级之分,`~/dev/*`的优先级高于`!~/*`.所以结合起来就是,除了`~/dev/`目录下的`.ycm_extra_conf.py`,home目录下所有的`.ycm_extra_conf.py`都不会被加载

#### 一个示例`.ycm_extra_conf.py`
以下是根据自己系统配置的`.ycm_extra_conf.py`,作为`g:ycm_global_ycm_extra_conf`指向的文件
这是一个 C 语言家族的 `.ycm_extra_conf.py`,其它语言写法类似,但有所不同,见官网
```pythonomnifunc
# 参考的.ycm_extra_conf.py,可以作为
flags = ['-x',  # -x + 语言不要漏
        'c++',
        '-Wall',
        '-Werr',
        '-isystem',
        '/usr/include',
        '-isystem',
        '/usr/include/c++/5.4.0',
        '-I',
        '/home/thinker/code/commlib/boost_1_68_0/mybuild/include/']
# 一般可以到/usr/include,/usr/local/include/ 下找到C和C++标准库的头文件,把它加进来,不需要重复添加
def Settings( **kwargs ):
  return {
    'flags':flags 
  }
```
