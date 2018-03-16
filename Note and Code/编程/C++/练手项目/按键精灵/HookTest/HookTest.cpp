// HookTest.cpp : ���� DLL Ӧ�ó���ĵ���������
//
#include<windows.h>
#include "stdafx.h"
#define _cplusplus _cpp  //�Լ�����һ��_cplusplus����Ϊ�˼���C��C++
#include"Hooktest.h"

LRESULT CALLBACK KeyboardProc1(int code,WPARAM wParam,LPARAM lParam)
{
	HWND hwnd_Imitate =FindWindow(NULL,L"��������");

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

//EXPORT�Ķ�����Hooktest.h�У�Ҳ����ֱ����extern"C" __declspec(dllexport)��������������
/*���붼�ᱻִ�б��룬������ֻ�б��EXPOR,��Textern"C" __declspec(dllexport)�ĺ����ű����룬�����EXPORT�ĺ�����ʾ�ɹ����ⲿʹ�õ�
�����⿪�ŵģ�����δEXPORT��ͬ�������������dll�У�ֻ�����ⲿ���ɵ���*/
EXPORT HHOOK HookKeyBoard()	//���ܣ�ע��һ�����̹���
{
	HWND hwnd_Imitate =FindWindow(NULL,L"��������");
	if (hwnd_Imitate==NULL)		//hwnd����NULL���жϵģ�MessageBox�ĵ�һ������������NULL
	{
		MessageBox(NULL,TEXT("�Ҳ�������"),L"ERROR",MB_OK);
		return NULL;
	}
	DWORD Th_id=GetWindowThreadProcessId(hwnd_Imitate,NULL);
	return SetWindowsHookEx(WH_KEYBOARD,&KeyboardProc1,GetModuleHandle(TEXT("HookTest.dll")),Th_id);//����һ��GetModuleHandleA��GetModuleHandle
	//����MSDN,SetWindowsHookEx������ĸ�����(�߳�ID)ָ��Ĳ��Ǳ����̵��̣߳���ô�ڶ�������ָ���HookProcedure(���Ӻ���)��������dll��
	//��������Ǳ��̵߳���Ϣ(���ĸ��߳�IDָ���̻߳����)����ô�ڶ�������ָ��Ĺ��Ӻ������Բ���д��dll��
	//Ҫ��ȡ���̵߳���Ϣ����������������ΪNULL����
}

//


