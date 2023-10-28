参考文章: https://www.jianshu.com/p/46e9b8a6cb6a

### find_package 原理
cmake本身不提供任何搜索库的便捷方法,要获得头文件路径和库路径最后还得有相应的 cmake 代码指向具体的路径,如果假如自己写的库想要被`find_package` 命令获取,就必须自己自己实现 FindXXX.cmake和XXXConfig.cmake. 调用了FindXXX.cmake和XXXConfig.cmake 的代码是find_packge命令能够获取头文件和库路径的根本原因

find_package采用两种模式搜索库
- **Module模式**：搜索CMAKE\_MODULE\_PATH指定路径下的FindXXX.cmake文件，执行该文件从而找到XXX库。 具体查找库并给XXX\_INCLUDE\_DIRS和XXX_LIBRARIES两个变量赋值的操作由FindXXX.cmake模块完成
- **Config模式**: 搜索XXX\_DIR指定路径下的XXXConfig.cmake文件，执行该文件从而找到XXX库. 搜索XXX_DIR指定路径下的XXXConfig.cmake文件，执行该文件从而找到XXX库



从上面来看,两种模式并无差别,只是文件名字不同,最终都是给 XXX\_INCLUDE\_DIRS 和 XXX\_LIBIRARIES 赋值,这里 cmake 默认会使用Module模式, Module 模式查找失败,才会使用 Config 模式.
cmake 默认到系统路径下查找 FindXXX.cmake 和 XXXConfig.cmake,所以如果把库和相应的cmake文件放在系统路径下,则在`CMakeList.txt`中直接使用`find_package`命令即可.但如果库是放在自定义的路径下,那么则要告诉 cmake 去哪找 FindXXX.cmake 和 XXXConfig.cmake 了. 比如我们把Caffe库安装到自定义路径下,那么如果要使用 CaffeConfig.cmake,则要在 CMakeList.txt 中加一句
```bash
set(Caffe_DIR /mydir/caffe/build)   #添加CaffeConfig.cmake的搜索路径
```

当安装好cmake后,对于一些经常使用的库,比如boost pthread,Cmake自带了一些.cmake文件,放在`/usr/share/cmake-3.5/Modules`下(cmake目录名称和版本号有关),我们直接在CMakeLists.txt中使用`find_package`时,如果没有指定自定义的cmake文件,则会在自带的cmake中查找,如果我们也没有为CMakeLists.txt中额外设置库的路径,则这些自带的cmake文件会到系统路径 /usr 下去查找这些库.下面举个例子

### 以Boost为例子
首先看看我写的一个C++ demo
```cpp
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <boost/asio.hpp>
  
using namespace std;                                                            

void print(const boost::system::error_code& e)
{
    std::cout << "hello world!" << std::endl;
}

int main(int argc, char *argv[])
{
        boost::asio::io_service io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(3));
        timer.async_wait(&print);
        sleep(3);
        std::cout << "After sleep 3s" << std::endl;
        sleep(2);
        std::cout << "After sleep 2s" << std::endl;
        io.run();
}
```
下面是 CMakeLists.txt 的内容
```cmake
cmake_minimum_required(VERSION 3.0)
SET(Boost_NO_SYSTEM_PATHS TRUE)
SET(Boost_INCLUDE_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/include) #自定义的include路径
SET(Boost_LIBRARY_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/lib)   #自定义的 lib 路径
find_package(Boost COMPONENTS system filesystem REQUIRED)  #把 boost_system 库和 boost_filesystem 库给链接进来
find_package(Threads) # 这里没有做自定义设置,会添加系统的 pthread 库         
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
                                                                             
add_executable(asio_test asio_test.cpp)
target_link_libraries( asio_test ${Boost_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})
```
asio\_test.cpp是我自己写的一个测试文件,这个文件要运行需要依赖`boost_system` 库和 `pthread`库,使用,在 CMakeLists.txt中,`find_package(Boost COMPONENTS system filesystem REQUIRED)`一句把 `boost_filesystem` 也给链接进来了,其实在此例中没有必要,但是我为了演示如何链接多个库所以也加上了

```cmake
SET(Boost_NO_SYSTEM_PATHS TRUE)
SET(Boost_INCLUDE_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/include) #自定义的include路径
SET(Boost_LIBRARY_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/lib)   #自定义的 lib 路径
```
意味着到我自定义的目录去查找,至于为什么要设置这些变量,可以打开`/usr/share/cmake-3.5/Modules`的`FindBoost.cmake`去查看这些变量的含义
对于`find_package(Boost COMPONENTS system filesystem REQUIRED)`,因为我们这里没有指定自定义的`FindBoost.cmake`,就会去执行`/usr/share/cmake-3.5/Modules`的`FindBoost.cmake`,但同时上面的SET设置使其不会到系统库路径去找boost,而是到我自己安装boost的指定路径下去找头文件和boost库
`find_package(Threads)`执行了`/usr/share/cmake-3.5/Modules`下的`FindThreads.cmake`因为没有额外自定义设置,因此链接系统的 pthread 库

接下来,在 CMakeLists.txt 所在目录下`mkdir mybuild && cd mybuild`,`cmake ..`,`make`就可以编译出文件了

### 拓展
XXXConfig.cmake的默认搜索路径不止一个，它们有详细的优先级顺序。如何生成FindXXX.cmake或XXXConfig.cmake文件更是一个复杂工程.后续再展开讲
