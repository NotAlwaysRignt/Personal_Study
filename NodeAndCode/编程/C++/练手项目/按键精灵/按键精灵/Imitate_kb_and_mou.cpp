#include"stdafx.h"	//要写上这个，不知道为什么,而且要写在最上头
// 解释：stdafx.h中没有函数库，只是定义了一些环境参数，使得编译出来的程序能在32位的操作系统环境下运行。 
#include"Imitate_kb_and_mou.h"
#include<windows.h>
#include<stdio.h>
#include<ctype.h>

class Imitate_kb_and_mou;//类要声明
Imitate_kb_and_mou::~Imitate_kb_and_mou()
{
	Info_list.clear();
}

bool Imitate_kb_and_mou::Lmouse_record()
{
	POINT point_Lbutton;	
	TCHAR mouxtext[20],mouytext[20];
	HWND parent_handle,moux,mouy;
	parent_handle=FindWindow(NULL,L"按键精灵");
	moux=FindWindowEx(parent_handle,NULL,NULL,L"x坐标");
	mouy=FindWindowEx(parent_handle,NULL,NULL,L"y坐标");
	
	if(GetCursorPos(&point_Lbutton))		//MSDN中GetCursorPos的参数时一个指针，但不要直接用野指针作为参数，而应是给变量的地址
	{
		wsprintf(mouxtext,L"%d",point_Lbutton.x);
		wsprintf(mouytext,L"%d",point_Lbutton.y);
		SetWindowText(moux,mouxtext);
		SetWindowText(mouy,mouytext);
	}
	else
	{
		MessageBoxA(parent_handle,"获取鼠标失败",NULL,MB_OK);
		return false;
	}
	
	
	int x=point_Lbutton.x;	//使字符串变为整形数据
	int y=point_Lbutton.y;
	if(x&&y)
	{
		POINT mousecontent;
		mousecontent.x=x;
		mousecontent.y=y;

		pInfo tmp=new Info();
		tmp->Infotype=Lmousebutton;
		tmp->Info_content.mousecontent=mousecontent;
		
		Info_list.push_back(tmp);
	}
	else
	{
		return false;
	}
	return true;

}

bool Imitate_kb_and_mou::Rmouse_record()
{
	POINT point_Rbutton;	
	TCHAR mouxtext[20],mouytext[20];
	HWND parent_handle,moux,mouy;
	parent_handle=FindWindow(NULL,L"按键精灵");
	moux=FindWindowEx(parent_handle,NULL,NULL,L"x坐标");
	mouy=FindWindowEx(parent_handle,NULL,NULL,L"y坐标");
	if(GetCursorPos(&point_Rbutton))		//MSDN中GetCursorPos的参数时一个指针，但不要直接用野指针作为参数，而应是给变量的地址
	{
		wsprintf(mouxtext,L"%d",point_Rbutton.x);
		wsprintf(mouytext,L"%d",point_Rbutton.y);
		SetWindowText(moux,mouxtext);
		SetWindowText(mouy,mouytext);
	}
	else
	{
		MessageBoxA(parent_handle,"获取鼠标失败",NULL,MB_OK);
		return false;
	}
	
	
	int x=point_Rbutton.x;	//使字符串变为整形数据
	int y=point_Rbutton.y;
	if(x&&y)
	{
		POINT mousecontent;
		mousecontent.x=x;
		mousecontent.y=y;

		pInfo tmp=new Info();
		tmp->Infotype=Rmousebutton;
		tmp->Info_content.mousecontent=mousecontent;
		
		Info_list.push_back(tmp);
	}
	else
	{
		return false;
	}
	return true;
}

bool Imitate_kb_and_mou::key_record()
{
	TCHAR keytext[2];
	HWND parent_handle,hwndkey;
	parent_handle=FindWindow(NULL,L"按键精灵");
	hwndkey=FindWindowEx(parent_handle,NULL,NULL,L"按键");
	
	GetWindowText(hwndkey,keytext,2);
	char ckey=toupper(keytext[0]);
	
	if(ckey>='A'&&ckey<='Z')
	{
		pInfo tmp=new Info();
		tmp->Infotype=Keyboard;
		tmp->Info_content.key=ckey;
		Info_list.push_back(tmp);
	}
	else
	{
		MessageBox(parent_handle,TEXT("输入错误"),TEXT("error"),MB_OK);
		return false;
	}
	return true;
}


bool Imitate_kb_and_mou:: Info_indicate()
{
	list<pInfo>::iterator iter=Info_list.begin();
	for(;iter!=Info_list.end();iter++)
	{
		switch((*iter)->Infotype)
		{
		case Keyboard:
			Sleep(3000);
			keybd_event((*iter)->Info_content.key,0,0,0);
			keybd_event((*iter)->Info_content.key,0,KEYEVENTF_KEYUP,0);
			break;
		case Lmousebutton:
			SetCursorPos((*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y);
			Sleep(3000);
			mouse_event(MOUSEEVENTF_LEFTDOWN,(*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,(*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y,0,0);
			break;
		case Rmousebutton:
			SetCursorPos((*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y);
			Sleep(3000);
			mouse_event(MOUSEEVENTF_RIGHTDOWN,(*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y,0,0);
			mouse_event(MOUSEEVENTF_RIGHTUP,(*iter)->Info_content.mousecontent.x,(*iter)->Info_content.mousecontent.y,0,0);
			break;
		default:break;
		}
	}
	return true;
}
bool Imitate_kb_and_mou::show_record()
{
	unsigned int keynum=0,Lbunum=0,Rbunum=0;
	HWND parent_handle,show_handle;
	
list<pInfo>::iterator iter=Info_list.begin();
	for(;iter!=Info_list.end();iter++)
	{
		pInfo tmp=*iter;
		switch(tmp->Infotype)
		{
		case Keyboard:
			keynum++;
			break;
		case Lmousebutton:
			Lbunum++;
			break;
		case Rmousebutton:
			Rbunum++;
			break;
		default:break;
		}		
	}
	TCHAR strmessage[50];
	wsprintf(strmessage,L"按键%d,左键%d,右键%d",keynum,Lbunum,Rbunum);
	parent_handle=FindWindow(NULL,L"按键精灵");
	show_handle=FindWindowEx(parent_handle,NULL,NULL,L"显示指令");

	SetWindowText(show_handle,strmessage);
	return true;
}

bool Imitate_kb_and_mou::clear_record()
{
	HWND parent_handle,hwndkey,show_handle,moux_handle,mouy_handle;
	parent_handle=FindWindow(NULL,L"按键精灵");
	hwndkey=FindWindowEx(parent_handle,NULL,NULL,L"按键");
	
	show_handle=FindWindowEx(parent_handle,NULL,NULL,L"显示指令");
	moux_handle=FindWindowEx(parent_handle,NULL,NULL,L"x坐标");
	mouy_handle=FindWindowEx(parent_handle,NULL,NULL,L"y坐标");
	Info_list.clear();	
	SetWindowText(hwndkey,L"");
	SetWindowText(show_handle,L"当前未存储指令");
	SetWindowText(moux_handle,L"");
	SetWindowText(mouy_handle,L"");
	return true;
}