#ifdef _cplusplus 
#define EXPORT extern"C" __declspec(dllexport)	//declspec��ž���declare specific��
#else
#define EXPORT __declspec(dllexport)
#endif
//ʹ���ܼ���C��C++

#include<windows.h>

EXPORT HHOOK HookKeyAndMouse();