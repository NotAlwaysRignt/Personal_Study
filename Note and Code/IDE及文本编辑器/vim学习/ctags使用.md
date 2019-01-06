ctag 是对代码进行解析然后生成补全信息的工具,常和 vim 搭配使用

### ctag文件命名
默认生成文件名为 tags, 建议的做法是 .tag,这样就是隐藏文件,不会污染项目

### 为C++标准库生成路径
C++标准库头文件没有.h后缀,当我们使用 ctags 生成 tag 文件时如果不特殊指定选项,没有.h后缀的文件不会被计入.
为了生成标准库头文件,我们需要加上`--language-force=c++`这个选项
网上有的说法说是使用`--force-language`,至少我的ctags 是行不通的,我们可以用`ctags --help`看是否有这个选项

### 使用 -I 选项
系统函数类似`extern int listen (int __fd, int __n) __THROW;`可能不会被加入到 tags 里
原因是__THROW的宏定义让ctags不再认为该系列函数是函数,如果要解决这个问题,我们在使用 ctags 时应该加上 -I 选项
举例
```bash
ctags -I __THROW -I __attribute_pure__ -I __nonnull -I __attribute__ --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c-kinds=+p --c++-kinds=+p --fields=+iaS --extra=+q -R -f mytags /usr/include /usr/local/include
```
### 为一些系统库头文件生成tag时不要用 -R
-R会递归生成文件,对于系统库头文件,会生成很大的tag,而且同名函数冲突严重,为了解决这个问题还是勤奋一点,不要用 -R,而是逐个指定目录
```
ctags --languages=c,c++ --links=yes --c-kinds=+p --c++-kinds=+p --fields=+iaS --extra=+q  -f ~/.vim/mytags /usr/include/* /usr/include/sys/* /usr/include/bits/*  /usr/include/netinet/* 
```
使用`/dir/*`可以遍历到那个前目录下的所有文件,但不会递归搜索,这样就可以指定我们想要生成 tag 的目录了

### ctag生成绝对路径
默认情况下ctag生成的是相对路径的 tag 文件,如果把这个 tag 文件放到其他目录下就会出现使用不了的情况,为了解决这个问题,可以让 ctag 生成绝对路径文件,这样在同一台机器上还是可以跳转的
```bash
ctags -f .tags --c++-kinds=+p --fields=+iaS --extra=+q -R `pwd`
```
**注意 -R 的顺序, -R 选项放最后, 否则虽然可以生成 tag 但会出现 tag 不全的情况 ,-R 后跟`pwd` 表示绝对路径**

### 对一些常用选项的说明
```bash
ctags -f .tags --c++-kinds=+p --fields=+iaS --extra=+q -R `pwd`
```
以上面命令为例
`–fields=[+|-]flags`
–fields指定tags的可用扩展域（extension fields），以包含到tags入口。
i:继承信息Inheritance information
a：类成员的访问控制信息 Access (or export) of class members
S： 常规签名信息，如原型或参数表 Signature of routine(e.g. prototype or parameter list)
`–extra=[+|-]flags`
指定是否包含某种扩展信息到tags入口。
q：包含类成员信息（如c++,java,Eiffel)。
但就算是C 语言的结构，也需要这两个参数设置才能获取成员信息。

这样就能自动完成结构和类的成员了

