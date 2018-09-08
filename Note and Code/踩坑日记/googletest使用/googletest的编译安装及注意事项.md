> 为了向公司的开发机上配置googletest,我到github上下载了 googletest 和googlemock 并进行安装,但中途遇到了不少问题,这里做下记录

公司的开发环境支持 c++0x 而不支持 c++11,虽然 c++0x 是 c++11 的前身,但是还是有区别的,这次编译遇到的问题就是来自于此

#### 编译
在github上下载的googletest-master 下包含两个主要目录,googletest 和googlemock,里面放置了CMakeLists.txt 文件,编译过程很简单,比如我们要编译googletest,只需要进入googletest目录

```bash
mkdir mybuild
cd mybuild #产生的Cmake 相关文件会存在mybuild目录下而不会污染整个工程
cmake .. #CMakeList.txt 在上一层目录
```
顺利的话就会在mybuild 产生编译后需要的库文件(.a)文件

编译顺利(编译最新版的googlemock时出现了问题,主要是编译选项设置的原因,要把-Werror去掉,见另一篇文章,不过其实即便解决了这个问题,最新的googletest也没办法使用)

#### 这里举例怎么使用gtest,gmock是一样的
库文件,编译好的库文件有两个,一个是libgtest.a ,这个是最主要的,另一个是libgtest_main.a,这个是可以省去我们写main函数时间用的,也可以不用,自己写main函数  

googletest的头文件放在 googleetest 的 include 目录下,我们把 include 里的gtest目录及其内部所有内容 拷贝过来即可使用(**gtest这个目录也要拷贝过来且名字不能改,因为头文件里都是用#include"gtest/xxx.h"写死了**)
编译选项: -I指向gtest以及**gtest所在目录**,-L指向库即可

但是使用gtest时却出现了问题
```bash
#问题出在gtest-internal.h
错误：‘declval’不是‘std’的成员
错误：expected primary-expression before ‘const’
错误：expected primary-expression before ‘>’ token
....
```
declval 是 C++11 后才添加到 std的,是新特性,googletest 1.9.0后强制要求C++11,如果我们编译时不支持 c++11(即使用 -std=c++11),那么就没办法使用这个头文件

于是找到github,选择 1.8.0 (上一个版本),重新编译,这次一路顺利,只要头文件和库文件包含正确,即可使用