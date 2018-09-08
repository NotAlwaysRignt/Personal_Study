##### 安装步骤
googletest 和 googlemock 的安装步骤是一样的
进入目录下,创建 mybuild 目录,然后进入使用cmake,这样产生的一堆Cmake相关文件才不会污染原来的工程
```bash
mkdir mybuild
cmake ..
make
```
编译好后libgtest.a libgtest_main.a(libgmock.a  libgmock_main.a)
会放在mybuild 目录下,要使用这个的库我们还要到项目目录下的include目录把头文件都包含进来,具体使用见官网教程

##### 编译时产生的具体问题
使用make时,出现
```bash
cc1plus: warnings being treated as errors
In file included from /data/home/chaoranxu/code/googletest/googlemock/include/gmock/gmock-spec-builders.h:70,
                 from /data/home/chaoranxu/code/googletest/googlemock/include/gmock/gmock-generated-function-mockers.h:43,
                 from /data/home/chaoranxu/code/googletest/googlemock/include/gmock/gmock.h:61,
                 from /data/home/chaoranxu/code/googletest/googlemock/src/gmock-all.cc:40:
/data/home/chaoranxu/code/googletest/googlemock/include/gmock/gmock-matchers.h: In member function ‘testing::internal::PointwiseMatcher<TupleMatcher, RhsContainer>::operator testing::Matcher<T>() const’:
/data/home/chaoranxu/code/googletest/googlemock/include/gmock/gmock-matchers.h:3010: 错误：‘use_UnorderedPointwise_with_hash_tables’的声明隐藏了‘this’的一个成员
make[2]: *** [CMakeFiles/gmock.dir/src/gmock-all.cc.o] 错误 1
make[1]: *** [CMakeFiles/gmock.dir/all] 错误 2
make: *** [all] 错误 2
```
具体原因暂未知,可能是gcc版本过低导致

解决办法:
出错关键在于:cc1plus: warnings being treated as errors
也就是警告当成错误处理,我们只要把编译时的 -Werror 选项去掉,不把警告当错误处理即可

```bash
$grep -r Werror ./
./gtest/CMakeFiles/gtest_main.dir/flags.make:CXX_FLAGS =    -Wall -Wshadow -Werror -DGTEST_HAS_PTHREAD=1 -fexceptions -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
./gtest/CMakeFiles/gtest.dir/flags.make:CXX_FLAGS =    -Wall -Wshadow -Werror -DGTEST_HAS_PTHREAD=1 -fexceptions -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
./CMakeFiles/gmock.dir/flags.make:CXX_FLAGS =    -Wall -Wshadow -Werror -DGTEST_HAS_PTHREAD=1 -fexceptions -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
./CMakeFiles/gmock_main.dir/flags.make:CXX_FLAGS =    -Wall -Wshadow -Werror -DGTEST_HAS_PTHREAD=1 -fexceptions -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
[chaoranxu@devnet_10_10_tlinux_64 mybuild]$vim CMakeFiles/gmock.dir/flags.make
```
用grep 找到 Werror的文件,经实践只要把  
CMakeFiles/gmock.dir/flags.make  
中的 -Werror 去掉即可,make 编译通过(当然会有警告)