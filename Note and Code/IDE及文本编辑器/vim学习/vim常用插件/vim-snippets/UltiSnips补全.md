### UltiSnips模板
UltiSnips 自带的模板很多,通常情况下它已经可以满足我们的要求.事实上要查看当前UltiSnips 版本自带的模板有哪些很简单,这些模板就放在 ~/.vim 的一个 vim-snippets/UltiSnips/ 目录中, 假设 我们使用plug 插件安装了 UltiSnips 插件,默认定义的模板存放在
```bash
~/.vim/plugged/vim-snippets/UltiSnips/c.snippets  # c模板
~/.vim/plugged/vim-snippets/UltiSnips/cpp.snippets  # c++模板
...
```
**使用C++时,默认也会启用 c 的模板**
有时我们会希望自定义一些模板,我们可以直接修改其默认的模板,但这并不推荐,因为插件一旦更新,原来的文件就会被覆盖掉,最好的方法是独创建一个 UltiSnips 文件夹来保存自定义代码片段。
```bash
# UltiSnips 插件会自动加载这个文件夹下的文件
mkdir ~/.vim/UltiSnips
```
假如你想定义一个所有语言通用的代码块，在所有的文件类型中都能用，你可以把它定义在 all.snippets 中,.snippets的命名是有讲究的ruby.snippets 中定义的代码块只会在 rb 文件中生效；python.snippets 中定义的代码块只会在 py 文件中生效。具体命名可参照默认的.snippets文件

下面举个使用markdown的例子
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
${0} 代表 tab 最终停留的位置， b 代表 begin (snippet should be expanded only at the beginning of a line)。


下面介绍下C++ 常用的模板,插件版本不同可能会有出入,具体见vim-snippets/UltiSnips 下的.snippets 文件
## C++
对于C++ ,UltiSnips 已经实现了一些补全,这些补全功能足以大大提高我们的效率,输入关键字然后按 < tab > 键,即可快速展开


##### 头文件
inc + < tab >会展开成 #include <stdio.h> 其中 stdio 被选中,可直接覆盖

incc + < tab > 会展开成 #include <iostream> 其中iostream 被选中,可直接覆盖

once + < tab > 这个在定义头文件时非常常用
```cpp
#ifndef filename
#defien filename

#endif /* end of include guard: _DATE_H_ */
```
##### cout
cout 会展开成 std::cout<< << endl; 光标位于两个 <<之间

##### main函数
main 会展开成
```cpp
int main(int argc, char *argv[])
{
	//光标在此
	return 0;
}
```
##### UltiSnips 跳转
UltiSnips 生成代码后有多处支持直接输入覆盖,比如一个class,我们可以修改类名,修改类名的同时构造函数,析构函数名字也会被自动同时替换,要修改的地方有多出, UltiSnips 支持快速跳转到某些指定位置,快速跳转的位置不妨称作定位符,对于自定义的模板,定位符可以自己设置
```
<c-j> " 跳至下一个部分   //c 即 <ctrl>
<c-k> " 跳至前一个部分
```

##### 函数
fun + < tab > 展开如下,其中 void function_name,参数,即{}内部都可以用快捷键快速跳转到并选中覆盖
```cpp
void function_name()
{

}
```

##### 类 class
cla + < tab >
```cpp
//设文件名为 test_mysql.h
#ifndef TEST_READ_MYSQL_H
#define TEST_READ_MYSQL_H

class TestReadMysql
{
private:


public:
    TestReadMysql();
    virtual ~TestReadMysql();
};

#endif /* TEST_READ_MYSQL_H */
```
上面的模板，class 默认值和文件名相同，并且会自动去掉下划线，非常有用
还有一种，比较不常用
cl + < tab >
```cpp
class test_read_mysql
{
public:
    test_read_mysql (arguments);
    virtual ~test_read_mysql ();

private:
    /* data */
}; 
```

##### 结构体 struct
st + < tab >
```
struct 文件名{

};
```

#### C++ 关键字均会展开
if  展开
```cpp
if(光标在此){

}
```

wh  +tab 可展开成 while ...
switch + tab 可展开 switch 的结构

**for有两种展开用法**
for + < tab > 可展开为
```cpp
for (i = 0; i < count; ++i) {

}
```
我们可以把变量i,count替换掉(可用 ctrl> + <j 或者 k> 来跳转)
上面模板的一个问题是,如果我们要在 i = 0 前面加一个 int,那么后面的 i<count, ++i 中的i也会跟上int

还有一种是
fori + < tab > (**注意for后面跟一个i**)
```cpp
for (int i = 0; i < count; ++i) {

}
```

对于 for switch while 的展开,我们往往要进行修改,记住下面两个快捷键即可

