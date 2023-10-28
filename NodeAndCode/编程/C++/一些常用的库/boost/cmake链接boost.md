cmake 安装好后,我们可以在`/usr/share/cmake-3.5/Modules`中找到内置的`FindBoost.cmake`,因此直接使用`find_package(Boost)`是可以调用到`FindBoost.cmake`内置的文件,如果没有做其它设置,那么cmake将在系统默认路径下寻找 Boost,但如果我们将 boost 安装到自定义的路径下,那么就需要做额外的设置了

#### 一个 C++ 例子
首先看看我写的一个C++ demo,demo的内容可以不用看,只要知道运行这个demo,必须依赖 boost_system 这个库,还要包含 boost 的头文件路径
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

#### 编写 CMakeLists.txt
boost 的安装不再介绍,现在假设已经编译并 install 了 boost,生成 `include` 和 `lib` 存放在 `/home/thinker/code/commlib/boost_1_68_0/mybuild/` 目录下,则 CMakeLists.txt 的内容如下

```bash
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

```bash
SET(Boost_NO_SYSTEM_PATHS TRUE) 
SET(Boost_INCLUDE_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/include) #自定义的include路径
SET(Boost_LIBRARY_DIR /home/thinker/code/commlib/boost_1_68_0/mybuild/lib)   #自定义的 lib 路径
```
意味着到我自定义的目录去查找,至于为什么要设置这些变量,可以打开`/usr/share/cmake-3.5/Modules`的`FindBoost.cmake`去查看这些变量的含义
对于`find_package(Boost COMPONENTS system filesystem REQUIRED)`,因为我们这里没有指定自定义的`FindBoost.cmake`,就会去执行`/usr/share/cmake-3.5/Modules`的`FindBoost.cmake`,但同时上面的SET设置使其不会到系统库路径去找boost,而是到我自己安装boost的指定路径下去找头文件和boost库
`find_package(Threads)`执行了`/usr/share/cmake-3.5/Modules`下的`FindThreads.cmake`因为没有额外自定义设置,因此链接系统的 pthread 库

接下来,在 CMakeLists.txt 所在目录下`mkdir mybuild && cd mybuild`,`cmake ..`,`make`就可以编译出文件了
