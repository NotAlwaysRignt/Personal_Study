### 介绍
该项目展示了系统库中的 sleep 函数和 free 函数是如何被 hook.cpp 的函数 hook 的

编译项目 `make`
运行最终生成的可执行文件 `main.out`

运行显示如下
```bash
This is Hook API sleep
This is Hook API free
```
则证明hook成功,sleep 和 free 被替换为 hook.cpp 中的 API

### hook 总结
#### hook 的实现方法
Linux C++ hook 系统API 有两种写法,一种是先`#include` 要hook的函数所在的头文件,然后再自己去实现它
另一种方式是不需要`#include`,但声明被hook的函数(与系统库的声明一致),并使用`extern "C"`声明,注意`extern "C"`必须加上,这样才能确保自定义的函数在编译器生成的符号和系统C库中的符号是相同的,如果没有这句话,以C++的规则,则会生成另一种符号,hook失败

以该项目为例,该项目 hook 了系统的 sleep 和 free 两个方法,替换为自定义的函数
Hook 系统的 sleep 和 free 有两种实现方式 * 第一种,在hook.cpp 中一定要包含`<unistd.h>`(sleep所在头文件) 和 `<stdlib.h>`(free所在头文件),这是该项目的实现方式
第二种方式,不需要在 hook.cpp 中包含系统函数的头文件,但要在头文件中声明 sleep 和 free 两个函数并使用 extern "C",项目代码注释掉的部分展示了要如何书写
如果没有使用 extern "C",实验证明 hook 失败,最终还是会调用系统的函数

#### 明确编译时要如何链接
在glibc 中`sleep`和`free`都是运行时被加载的,这是 hook 可以实现的前提.而我们自定义的`hook`函数并不要求一定被编译成动态库,事实上也可以不用特地编译成静态库的形式
通过Makefile 可知,只要在最终编译可执行文件时将`hook.o` 链接进来即可成功实现`Std.o`中的函数被hook
综上,我们实现了 hook 的代码,只要确保编译时被链接进来即可成功实现hook,因为glibc总是运行时被链接进来,因此总能实现符号的替换
但要注意的是,有些库也有一些系统API的实现,比如`-lpthread`中就有 socket 符号,因此我们要确保自定义的 hook 库在编译链接时位置要放在`pthread`库的前面
