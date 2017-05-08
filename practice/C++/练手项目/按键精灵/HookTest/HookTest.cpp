// HookTest.cpp : 定义 DLL 应用程序的导出函数。
//
#include<windows.h>
#include "stdafx.h"
#define _cplusplus _cpp  //自己创建一个_cplusplus宏是为了兼容C和C++
#include"Hooktest.h"

LRESULT CALLBACK KeyboardProc1(int code,WPARAM wParam,LPARAM lParam)
{
	HWND hwnd_Imitate =FindWindow(NULL,L"按键精灵");

	if(lParam&0x1000==0)
	{
			switch(wParam)
		{
		case VK_F11:
		SendMessage(hwnd_Imitate,WM_KEYDOWN,VK_F11,0);
		break;
		case VK_F12:
		SendMessage(hwnd_Imitate,WM_KEYDOWN,VK_F12,0);
		break;
		default:break;
		}
	}
	return CallNextHookEx(0,code,wParam,lParam);
}

//EXPORT的定义在Hooktest.h中，也可以直接用extern"C" __declspec(dllexport)，但看起来繁琐
/*代码都会被执行编译，并不是只有标记EXPOR,即Textern"C" __declspec(dllexport)的函数才被编译，标记了EXPORT的函数表示可供给外部使用的
即对外开放的，其它未EXPORT的同样被编译后存放在dll中，只不过外部不可调用*/
EXPORT HHOOK HookKeyBoard()	//功能，注册一个键盘钩子
{
	HWND hwnd_Imitate =FindWindow(NULL,L"按键精灵");
	if (hwnd_Imitate==NULL)		//hwnd是用NULL来判断的，MessageBox的第一个参数可以是NULL
	{
		MessageBox(NULL,TEXT("找不到窗口"),L"ERROR",MB_OK);
		return NULL;
	}
	DWORD Th_id=GetWindowThreadProcessId(hwnd_Imitate,NULL);
	return SetWindowsHookEx(WH_KEYBOARD,&KeyboardProc1,GetModuleHandle(TEXT("HookTest.dll")),Th_id);//尝试一下GetModuleHandleA和GetModuleHandle
	//根据MSDN,SetWindowsHookEx如果第四个参数(线程ID)指向的不是本进程的线程，那么第二个参数指向的HookProcedure(钩子函数)必须是在dll中
	//如果钩的是本线程的消息(第四个线程ID指向本线程或进程)，那么第二个参数指向的钩子函数可以不用写在dll中
	//要勾取本线程的消息，第三个参数设置为NULL即可
}

//


