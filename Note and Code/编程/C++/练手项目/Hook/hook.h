#ifndef HOOK_H
#define HOOK_H
#include <stddef.h>
// To hook sleep in <unistd.h> and free in <stdlib.h>
//
// 必须使用 extern "C",这样编译器才会按照C编译器的方式去解析符号,C标准库中的 free 和 sleep 会因符号相同而被hook
// 如果没有使用 extern "C",实验证明 hook 失败,最终还是会调用系统的函数
#ifdef  __cplusplus
extern "C" {
#endif

unsigned int sleep(unsigned int seconds);

//void *malloc(size_t size); //malloc的API不要hook,否则程序会报错"段错误"完全无法运行,原因未知
void free(void *ptr);

// 调用该函数使得该文件中的符号被强引用,而不是被跳过
int enable_hook();

#ifdef  __cplusplus
}
#endif

#endif /* HOOK_H */
