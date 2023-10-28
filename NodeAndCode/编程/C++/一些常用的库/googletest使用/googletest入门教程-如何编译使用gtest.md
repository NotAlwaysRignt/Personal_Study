##### 如何使用googletest
网上关于googletest的介绍很多,但事实上没几个在linux上能跑的案例,这里循序渐进,先介绍googletest最进本的使用,让它跑起来

##### 编译googletest
在github上下载下googletest,然后进行编译
如何编译见官网,用cmake是个不错的选择,毕竟跨平台,编译也非常简单,在根目录下(根目录下放置了CMakeLists)创建mybuild目录,进入mybuild目录去编译,这时因为执行cmake时会产生一大堆cmake相关的文件,为了避免这些文件污染工程所以要自己创建个目录进去cmake(这几乎是一种标准做法)
```bash
mkdir mybuild
cd mybuild
cmake ../
```
这个时候我们在mybuild 目录下会获得两个库
* libgtest.a
* libgtest_main.a
其中libgtest.a是最重要的,libgtest_main.a 主要是帮助我们省去了写main函数的麻烦,不过写main函数并不是什么麻烦事,链接多一个库倒是麻烦多了,所以一般把libgtest.a带上即可

要使用googletest,至少需要三样东西, **libgtest.a,googletest的头文件,系统的thread库**,所有相关头文件放在了googletest这个项目工程的include/gtest中,编译时我们需要把 gtest.h 包含进来,其它头文件gtest.h会去链接

第一次编译 gtest 有一些注意事项,包括指定 -I 参数(注意!), -L参数, 以及 -lpthread参数
最简单的demo
为了不让例子又关于简单,还是写一个类吧
我们写一个Add类 Add.cpp, main.cpp 放在同一个目录下,Add类就完成一个加法功能,目录结构如下(省去了很多头文件)
```cpp
.
├── Add.cpp
├── gtest
│   ├── gtest.h
│   ├── ...
│   └── internal
│       ├── custom
│       │   ├── gtest.h
│       │   ├── ...
│       ├── ...
├── lib
│   └── libgtest.a
├── main.cpp
└── main.out
```
先写个 Add.cpp
```cpp
#include<iostream>
using namespace std;
class Add
{
    public:
    Add(){cout<<"Add Init"<<endl;}
    int add_func(int a,int b){return a+b;}
};
```

再写一个main.cpp,注意 TEST 宏的两个参数 function 和 add 可以是任意字符,它主要是为了编译我们观察输出
main函数中的 InitGoogleTest 和 RUN_ALL_TESTS 几乎是做法,InitGoogleTest 允许我们在命令行执行可执行文件时输入参数, RUN_ALL_TESTS 会运行所有的 测试宏
```cpp
#include"gtest/gtest.h"
#include"Add.cpp"
TEST(funtion,add)
{
    Add a;
    EXPECT_EQ(1,a.add_func(0,1));
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
```
编译,然后运行
```bash
g++ main.cpp -I./ -L./lib -lgtest -lpthread -o main.out
./main.out
```
这里有几点要注意:
##### 注意 -I 参数
打开 gtest.h 文件,你会发现里面有诸如
```cpp
#include "gtest/internal/gtest-string.h"
#include "gtest/gtest-death-test.h"
```
等头文件包含项,这就意味着这种文件的包含关系被写死了,根据这个例子的目录结构,gtest.h是存放在 gtest 目录里而不是和gtest在同一个目录下(事实上即使把gtest.h 提取出来依然会有在其它头文件中出现路径包含出错的问题),如果我们不指定 -I./ 选项,那么它会提示找不到 gtest/internal/...h 文件
也就是说,我们使用 -I 寻找头文件时,必须让 -I 指定到gtest所在的目录(gtest.h,internal等放在gtest这个目录下)
###### 指定 -lpthread 选项
gtest依赖多线程库,这个是必须的,否则会出现
```bash
./lib/libgtest.a(gtest-all.cc.o)：在函数‘testing::internal::ThreadLocal<testing::TestPartResultReporterInterface*>::~ThreadLocal()’中：
gtest-all.cc:(.text._ZN7testing8internal11ThreadLocalIPNS_31TestPartResultReporterInterfaceEED2Ev[_ZN7testing8internal11ThreadLocalIPNS_31TestPartResultReporterInterfaceEED5Ev]+0x25)：对‘pthread_getspecific’未定义的引用
.........
```
反正就是一堆thread找不到,该想到这个问题了!

输出结果如下:
```bash
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from funtion
[ RUN      ] funtion.add
Add Init
[       OK ] funtion.add (0 ms)
[----------] 1 test from funtion (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (1 ms total)
[  PASSED  ] 1 test.

```
注意 Add Init 在[ RUN      ] funtion.add 下面,说明 cout 一样在测试中可以用,同时我们也知道Test(function,add) 中 function 和 add 可以是任意字符,且最终会以怎么样的形式打印

##### 编写一个Hello World程序
googletest可以
