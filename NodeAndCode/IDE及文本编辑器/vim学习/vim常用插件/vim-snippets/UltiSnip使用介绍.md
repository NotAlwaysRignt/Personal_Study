[TOC]
### UltiSnips 介绍
UltiSnips 是一个代码块引擎,我们可以通过书写少量字符串,然后通过快捷键就生成一大段特定文本,大大减小我们的书写工作
UltiSnips 自带的模板很多,通常情况下它已经可以满足我们的要求.事实上要查看当前 UltiSnips 版本自带的模板有哪些很简单,这些模板就放在 ~/.vim 的一个 vim-snippets/UltiSnips/ 目录中, 假设 我们使用plug 插件安装了 UltiSnips 插件,默认定义的模板存放在
```bash
~/.vim/plugged/vim-snippets/UltiSnips/c.snippets  # c模板
~/.vim/plugged/vim-snippets/UltiSnips/cpp.snippets  # c++模板
......
```

### UltiSnips 的学习资料
1. 进入 github 里 UltiSnips 的官网,`REAME.md`中有关于其中用法的介绍,这时最权威的资料
2. 下载插件后使用 vim 的自带文档,`:help UltiSnips`可以看到完整的 UltiSnips 使用介绍,再比如我们想查看 UltiSnips 是如何嵌入 python 的可以用`:help UltiSnips-python` 来查看
3. 查看 UltiSnips 自带的 `.snippets`文件,通过 UltiSnips 自带的默认模板,我们可以学习它是如何定义模板的
4. https://keelii.com/2018/08/26/vim-plugin-ultisnips-advanced-tips/ 
    是介绍 UltiSnips 的一篇不错的中文教程

### 语法介绍
先看一个简单的例子
```bash
snippet datetime "YYYY-MM-DD hh:mm" w
`!v strftime("%Y-%m-%d %H:%M")`
endsnippet
```
`snippet` 和 `endsnippet`构成了一个 snippet 块, `datetime` 是触发渲染模板的关键字,后面`"YYYY-MM-DD hh:mm"`是注释,它没有实际作用, w 是 片段参数
\`!v\`表示执行 vim-script,后面`strftime("%Y-%m-%d %H:%M")`是vim的内置函数,其它的还有 \`!p\`里面嵌入的是 python 脚本
我们输入`datetime <tab>`,然后 datetime 就会被渲染成`2018-12-10 20:52`

```bash
# ~/.vim/UltiSnips/markdown.snippets

snippet head "Jekyll post header" b
---
title: ${1:title}
layout: post
guid: `!p
import uuid
if not snip.c:
  guid = uuid.uuid4().get_hex()
snip.rv = guid
`
date: `!v strftime("%Y-%m-%d %H:%M:%S")`
tags:
  - ${4}
---

${0}
endsnippet
# vim:ft=snippets:
```
`${n}`代表占位符,我们可以使用快捷键跳转,默认`<c-j>`跳转到下一个占位符`<c-k>`跳转到上一个占位符.具体快捷键在`.vimrc`中设置,`${1}`是触发补全时光标会停留的第一个位置, `${0}`代表 tab 最终停留的位置， b 代表 begin (snippet should be expanded only at the beginning of a line)。

##### 代码片段的参数
* b 表示触发字符应该在一行的开始
* i 表示触发字符可以在单词内（连续展示会使用这个选项）
* w 表示触发字符的前后必须是一个字母分界点
* r 表示触发字符可以是一个正则表达式
* t 表示展开的代码片段中如果有制表符，原样输出，即使你的 vimrc 里面设置了 expandtab
* m 表示删除代码片段右边的所有空白字符
* e 表示自定义上下文
* A 表示自动触发，不需要按 tab，类似于 VIM 中的 abbr

### 嵌入 python
我们很容易找到嵌入 python 的例子,比如可以到 all.snippets 文件里查看
要使用python应该使用 \`!p {python-code}\`的形式,将代码写在两个\`之间

#### 定义python函数
如果python代码很长,那么全部写到\`!p\`里会很难阅读,因此可以考虑把函数剥离出来
```Python
global !p
def insert_word():
    snip.rv = "Hello world"
endglobal
```
python函数可以写到`global !p`和`endglobal`两个代码块之间
UltiSnips 定义了很多 内置对象,如 snip.rv 表示\`!p\`这个代码块最终表示的文本, `t[1]`表示`$1`这个占位符,`t[2]`表示`$2`这个占位符,以此类推

#### Ulsnipts自带的python对象
我们可以通过`:help UltiSnips`来查看相关python 用法,这里只介绍几个对象的用法
###### snip.rv
`snip.rv`,它是str类型,snip.rv的最终内容是插入到文本的内容,比如 `snip.rv="123456"  snip.rv="123"`最终插入的是"123"
###### snip.rv
它的类型`<class 'UltiSnips.text_objects._python_code.SnippetUtil'>`(我们可以用`snip.rv = str(type(snip)))`让它显示出来
在网上教程中几乎没有`snip`的介绍,多数是`snip.rv`的介绍,不过我们可以在自带的 all.snippets 中的 box 和 bbox 找到它的使用,这里简单讲解其用法
`snip = snip + "string"` 表示在新的一行嵌入"string",并且不会和原来的`snip.rv`冲突.
这里会有一些奇怪的坑,比如在一个占位符后多次进行`snip.rv`和`snip+="str"`的赋值会出现一些冲突,这里我还没查明具体原因,最好的解决办法就是实验调试,直到达到预期

#### 外部导入python
既然支持python,则肯定支持 import,这样对于代码量较大的情况我们可以单独成`.py`文件来组织
既然支持import,则我们得知道 UltiSnips import 的查找路径
一种方法是我们可以使用sys.path.append去添加路径再导入

另一种方法是借助 demo,比如打开 `all.snippets`
```Python
global !p
from vimsnippets import foldmarker, make_box, get_comment_format
endglobal
```
我们只要在 UltiSnips 插件目录中找到 vimsnippets.py 文件所在的位置,那个位置就是 import 的路径
### 使用自定义模板
有时我们会希望自定义一些模板,我们可以直接修改其默认的模板,但这并不推荐,因为插件一旦更新,原来的文件就会被覆盖掉,最好的方法是独创建一个 UltiSnips 文件夹来保存自定义代码片段。
```bash
# UltiSnips 插件会自动加载这个文件夹下的文件
mkdir ~/.vim/UltiSnips
```
Ulsnipts 会优先加载`~/.vim/UltiSnips`下的 snippets 文件,然后再加载默认的文件,如果`~/.vim/UltiSnips/`下的文件和默认的模板有冲突,则会使用优先使用我们自定义的模板

#### extends 关键字
`cpp.snippets`文件中使用了`extend c`,表示复用`c.snippets`下全部的代码块,这可以减少我们的工作量,比如`vue.snippets`就复用了 html, javascript, css 的 snippets

#### snippets 和文件命名的关系
假如你想定义一个所有语言通用的代码块，在所有的文件类型中都能用，你可以把它定义在`all.snippets`中,.snippets的命名是有讲究的,Ulsnipts 会根据 vim 属性`filetype`去找到对定的`.snippets`文件,比如当我们打开`.py`文件是,在vim 中输入`:set filetype`可以得到`filetype=python`因此,当在`.py`文件中触发补全时, Ulsnipts 会去查找`python.snippets`文件

#### 自定义文件类型
根据这个原理我们可以自定义文件类型,比如一个写一个`test.snippets`,然后我们创建一个`hello.test`文件,这时我们还不能补全,因为输入`:set filetype`时会发现`filetype`是空的,因此要在`hello.test`窗口内手动输入`:set filetype=test`,这样就可以使用`test.snippets`的补全了
当然每次手动设置`filetype`很麻烦,我们可以修改`.vimrc`设置,比如设置`.test`后缀文件filetype为 html :`au BufRead,BufNewFile *.test set filetype=html`


### 常用的模板
UltiSnips 自带的模板已经足够强大,我们可以记住一些常用的模板,提高工作效率,这里为了省略篇幅不会对补全后的效果做介绍,因为补全后的内容可能也会随着版本变化而更新,自己可以打开`.snippets`文件去看补全后的效果
##### C++
* `inc + <tab>`会展开成 `#include <stdio.h>` 其中 stdio 被选中,可直接覆盖
* `incc + <tab>` 会展开成 `#include <iostream>` 其中iostream 被选中,可直接覆盖
* `once + <tab>` 这个在定义头文件时非常常用
* `cout + <tab>` 会展开成 `std::cout<< << endl;` 
* `main + <tab>`展开成 main 函数
* `cla + <tab>` 展开类,并且带`#ifdef`和`#endif`结构,在头文件定义时很方便
* `if + <tab>``for + <tab>` `fori + <tab>` `wh + <tab>` `switch + <tab>`
