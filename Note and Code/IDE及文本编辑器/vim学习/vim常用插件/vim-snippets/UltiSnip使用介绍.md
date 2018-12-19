### 最佳的学习资料
1. 进入 github 里 UltiSnips 的官网, REAME.md 中有关于其中用法的介绍,这时最权威的资料
2. 下载插件后使用 vim 的自带文档,`:help snippet`可以看到完整的 UltiSnips 使用介绍,再比如我们想查看 UltiSnips 是如何嵌入 python 的可以用`:help UltiSnips-python` 来查看
3. 查看 UltiSnips 自带的 demo,我们可以看见 UltiSnips 已经自带了很多默认的模板,通过查看`.snippets`文件,我们可以学习它是如何定义模板的
4. https://keelii.com/2018/08/26/vim-plugin-ultisnips-advanced-tips/ 
    是介绍 UltiSnips 的一篇不错的中文教程
### 基本介绍

```shell
snippet datetime "YYYY-MM-DD hh:mm" w
`!v strftime("%Y-%m-%d %H:%M")`
endsnippet
```

`snippet` 和 `endsnippet`够整了一个 snippet 块, `datetime` 是触发渲染模板的块,后面`"YYYY-MM-DD hh:mm"`是注释,它没有实际作用, w 是 片段参数
\`!v\`表示执行 vim-script,后面`strftime("%Y-%m-%d %H:%M")`是vim的内置函数,其它的还有 \`!p\`里面嵌入的是 python 脚本
我们输入`datetime <tab>`,然后 datetime 就会被渲染成`2018-12-10 20:52`
#### 代码片段的参数
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
```shell
global !p
def insert_word()
    snip.rv = "Hello world"
endglobal
```
python函数可以写到`global !p`和`endglobal`两个代码块之间
UltiSnips 定义了很多 内置对象,如 snip.rv 表示\`!p\`这个代码块最终表示的文本, `t[1]`表示`$1`这个占位符,`t[2]`表示`$2`这个占位符,以此类推

#### 外部导入python
既然支持python,则肯定支持 import,这样对于代码量较大的情况我们可以单独成`.py`文件来组织
既然支持import,则我们得知道 UltiSnips import 的查找路径
一种方法是我们可以使用sys.path.append去添加路径再导入

另一种方法是借助 demo,比如打开 `all.snippets`
```shell
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
假如你想定义一个所有语言通用的代码块，在所有的文件类型中都能用，你可以把它定义在 all.snippets 中,.snippets的命名是有讲究的ruby.snippets 中定义的代码块只会在 rb 文件中生效；python.snippets 中定义的代码块只会在 py 文件中生效。具体命名可参照默认的.snippets文件
