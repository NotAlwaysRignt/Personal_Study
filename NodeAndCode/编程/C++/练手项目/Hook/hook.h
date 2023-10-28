#ifndef HOOK_H
#define HOOK_H
#include <stddef.h>
// To hook sleep in <unistd.h> and free in <stdlib.h>

/* 
 * Hook 系统的 sleep 和 free 分两种实现方式 * 第一种,在hook.cpp 中一定要包含<unistd.h>(sleep所在头文件) 和 <stdlib.h>(free所在头文件),这是该项目的实现方式
 * 第二种情况,不需要在 hook.cpp 中包含系统函数的头文件,但要在头文件中声明 sleep 和 free 两个函数并使用 extern "C"
 * 如果没有使用 extern "C",实验证明 hook 失败,最终还是会调用系统的函数
*/
//#ifdef  __cplusplus
//extern "C" {
//#endif

//unsigned int sleep(unsigned int seconds);

//void *malloc(size_t size); //malloc的API不要hook,否则程序会报错"段错误"完全无法运行,原因未知
//void free(void *ptr);

// 调用该函数使得该文件中的符号被强引用,而不是被跳过
int enable_hook();

//#ifdef  __cplusplus
//}
//#endif

#endif /* HOOK_H */
