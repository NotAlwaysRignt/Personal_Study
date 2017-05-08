// 线程池.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"//这个include应该写在第一个
#include"CmyPool.h"
#include<stdlib.h>
#include<windows.h>
int Double(char *pc="Here is double i",char c='\0',int *pi=nullptr,int i=1);
bool Delay(char *pc=nullptr,char c='\0',int* pi=nullptr,int i=0);
 int Double(char* pc,char c1,int* pi ,int i)
{
	printf("%s%d\n",pc,i*2);
	return i*2;
}

 bool Delay(char*,char,int *,int)
{
	Sleep(2000);
	printf("2s delay\n");
	return true;
}
int _tmain(int argc, _TCHAR* argv[])
{
	CmyPool Cpool(2);
	while(true)
	{
		char c=getchar();
		fflush(stdin);
		if(c!='q')
		{
				Cpool.dojob(Double,Delay);
				printf("%d\n",Cpool.getsize());
		}
		else
		{break;}
	}
	system("pause");
	GetLastError();
	return 0;
}

