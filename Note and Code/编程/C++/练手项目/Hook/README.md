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
1. `extern "C"`必须加上,这样才能确保自定义的函数在编译器生成的符号和系统C库中的符号是相同的,如果没有这句话,以C++的规则,则会生成另一种符号,hook失败

2. 通过Makefile 可知,只要在最终编译可执行文件时将`hook.o` 链接进来即可成功实现`Std.o`中的函数被hook

3. 在glibc 中 sleep 和 free 都是运行时被加载的,这是 hook 可以实现的前提.而我们自定义的hook函数并不要求一定被编译成动态库,比如本例中的`hook.o`就是静态链接进来的
