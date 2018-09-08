cppcheck 是静态代码检查工具,它不会检查语法错误,但可以对一些潜在的运行时错误进行检查
比如 new 了一个数组,delete 时却没有用 [], 再如 STL erase 导致迭代器失效,这些都可以检查出来,非常强大

##### 使用技巧
1. 检查单文件
```bash
cppcheck 文件名
```
2. 包含头文件的检查(很有必要),加上一个 -I 参数,指明头文件的路径
```bash
cppcheck test.cpp -Iinlcude
```
假设test.cpp 和 include目录都在同一目录下,且 test.cpp 中有
```cpp
#include"test.h"
```
test.h 放在include 目录下,那么cppcheck也会进入进行相应的检查