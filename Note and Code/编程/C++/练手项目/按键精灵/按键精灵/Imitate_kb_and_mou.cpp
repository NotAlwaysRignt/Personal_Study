#include"stdafx.h"	//Ҫд���������֪��Ϊʲô,����Ҫд������ͷ
// ���ͣ�stdafx.h��û�к����⣬ֻ�Ƕ�����һЩ����������ʹ�ñ�������ĳ�������32λ�Ĳ���ϵͳ���������С� 
#include"Imitate_kb_and_mou.h"
#include<windows.h>
#include<stdio.h>
#include<ctype.h>

class Imitate_kb_and_mou;//��Ҫ����
Imitate_kb_and_mou::~Imitate_kb_and_mou()
{
	Info_list.clear();
}

bool Imitate_kb_and_mou::Lmouse_record()
{
	POINT point_Lbutton;	
	TCHAR mouxtext[20],mouytext[20];
	HWND parent_handle,moux,mouy;
	parent_handle=FindWindow(NULL,L"��������");
	moux=FindWindowEx(parent_handle,NULL,NULL,L"x����");
	mouy=FindWindowEx(parent_handle,NULL,NULL,L"y����");
	
	if(GetCursorPos(&point_Lbutton))		//MSDN��GetCursorPos�Ĳ���ʱһ��ָ�룬����Ҫֱ����Ұָ����Ϊ��������Ӧ�Ǹ������ĵ�ַ
	{
		wsprintf(mouxtext,L"%d",point_Lbutton.x);
		wsprintf(mouytext,L"%d",point_Lbutton.y);
		SetWindowText(moux,mouxtext);
		SetWindowText(mouy,mouytext);
	}
	else
	{
		MessageBoxA(parent_handle,"��ȡ���ʧ��",NULL,MB_OK);
		return false;
	}
	
	
	int x=point_Lbutton.x;	//ʹ�ַ�����Ϊ��������
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
	parent_handle=FindWindow(NULL,L"��������");
	moux=FindWindowEx(parent_handle,NULL,NULL,L"x����");
	mouy=FindWindowEx(parent_handle,NULL,NULL,L"y����");
	if(GetCursorPos(&point_Rbutton))		//MSDN��GetCursorPos�Ĳ���ʱһ��ָ�룬����Ҫֱ����Ұָ����Ϊ��������Ӧ�Ǹ������ĵ�ַ
	{
		wsprintf(mouxtext,L"%d",point_Rbutton.x);
		wsprintf(mouytext,L"%d",point_Rbutton.y);
		SetWindowText(moux,mouxtext);
		SetWindowText(mouy,mouytext);
	}
	else
	{
		MessageBoxA(parent_handle,"��ȡ���ʧ��",NULL,MB_OK);
		return false;
	}
	
	
	int x=point_Rbutton.x;	//ʹ�ַ�����Ϊ��������
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
	parent_handle=FindWindow(NULL,L"��������");
	hwndkey=FindWindowEx(parent_handle,NULL,NULL,L"����");
	
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
		MessageBox(parent_handle,TEXT("�������"),TEXT("error"),MB_OK);
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
	wsprintf(strmessage,L"����%d,���%d,�Ҽ�%d",keynum,Lbunum,Rbunum);
	parent_handle=FindWindow(NULL,L"��������");
	show_handle=FindWindowEx(parent_handle,NULL,NULL,L"��ʾָ��");

	SetWindowText(show_handle,strmessage);
	return true;
}

bool Imitate_kb_and_mou::clear_record()
{
	HWND parent_handle,hwndkey,show_handle,moux_handle,mouy_handle;
	parent_handle=FindWindow(NULL,L"��������");
	hwndkey=FindWindowEx(parent_handle,NULL,NULL,L"����");
	
	show_handle=FindWindowEx(parent_handle,NULL,NULL,L"��ʾָ��");
	moux_handle=FindWindowEx(parent_handle,NULL,NULL,L"x����");
	mouy_handle=FindWindowEx(parent_handle,NULL,NULL,L"y����");
	Info_list.clear();	
	SetWindowText(hwndkey,L"");
	SetWindowText(show_handle,L"��ǰδ�洢ָ��");
	SetWindowText(moux_handle,L"");
	SetWindowText(mouy_handle,L"");
	return true;
}