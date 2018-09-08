googletest 使用很简单,只要把 库文件用 -L 和 -l 链接进来,头文件 gtest.h (如果要用mock,就把gmock.h)也包含进来,就可以开始写了

#### 运行指定的测试
googletest支持运行指定的测试,要指定某个测试运行,可以在命令行中运行时指定参数 --gtest_filter =
我们也可以在代码中指定运行哪些测试,然后编译运行,这两种情况各有优缺点,代码指定则意味着需要重新编译,命令行中加参数
则可以在编译后指定某些单元进行测试


例子是最好的说明
```cpp
#include "gtest.h"
TEST(mtthread,mt_time_ms)
{
	//...
}

TEST(time,gettime)
{
	//...
}

int main(int argc, char *argv[])
{
    //::testing::GTEST_FLAG(filter) = "mtthread.mt_time_ms";
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
```
TEST 宏的两个参数都可以自己随便定义,它们主要是用来对测试用例分类的,编译成可执行文件 test_get_current_time.out 后

### 在命令行中指定特定的单元测试来运行
我们可以用 ./test_get_current_time.out --help 查看有哪些选项
下面介绍最常用的几种:

##### --gtest_list_tests
显示这个单元测试中的测试单元列表,如上述例子中
```bash
./test_get_current_time.out --gtest_list_tests
mtthread.
  mt_time_ms
time.
  gettime
```

#####--gtest_filter
指定特定的单元测试运行,支持正则匹配,具体见 ./test_get_current_time.out --help
```bash
./test_get_current_time.out --gtest_filter=mtthread.mt_time_ms

# 会显示运行指定单元的结果
```
** 注意 --gtest_filter= 的等号前后不要有空格**
我们也可以用 : 作为分隔指定多个单元测试
```bash
./test_get_current_time.out --gtest_filter=mtthread.mt_time_ms:time.gettime
```

### 在代码中指定单元测试
如果我们要在代码中指定特定单元测试,只需要用 ::testing::GTEST_FLAG(filter) = "A.a"的形式

沿用上面的代码示例,我们只要稍微修改一下main函数,在前面加多一步
"mtthread.mt_time_ms" 的效果就和 --gtest_filter 的效果是一样的,同样支持正则匹配,对于多个测试用例,用:分隔即可
```cpp
int main(int argc, char *argv[])
{
    ::testing::GTEST_FLAG(filter) = "mtthread.mt_time_ms";
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
```