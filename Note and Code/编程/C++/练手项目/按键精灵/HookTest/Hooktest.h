#ifdef _cplusplus 
#define EXPORT extern"C" __declspec(dllexport)	//declspec大概就是declare specific吧
#else
#define EXPORT __declspec(dllexport)
#endif
//使其能兼容C和C++

#include<windows.h>

EXPORT HHOOK HookKeyAndMouse();