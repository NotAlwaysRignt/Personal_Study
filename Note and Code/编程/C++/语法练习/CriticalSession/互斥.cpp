// 互斥.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
/*学会使用CriticalSection(关键代码段)功能防止互斥现象*/
//主要用到的函数
/*
InitializeCriticalSection
DeleteCriticalSection
EnterCriticalSection
DeleteCriticalSection
*/
class Lock
{
public:
	Lock(int data=2);
	~Lock(); 
	static void IncreaseData(Lock *);
	static void DecreaseData(Lock *);

private:
	int m_data;
	Lock(const Lock&){};
	Lock& operator=(const Lock&){};
	CRITICAL_SECTION m_cr;
};

Lock::Lock(int data):m_data(data)
{
	::InitializeCriticalSection(&m_cr);
}

Lock::~Lock()
{
	::DeleteCriticalSection(&m_cr);
}

void Lock::IncreaseData(Lock* plock)
{
	Lock* lock =plock;	
	::EnterCriticalSection(&(lock->m_cr));
	lock->m_data+=1;
	Sleep(1);//如果在执行完lock->m_data+=1;时plock->m_data>>=1;被执行了，那么数据就会错误;
	//即使去掉Sleep(),这种错误也会发生。因此我们为了防止这种错误必须控制好时间片，即保证在执行完IncreaseData再执行DecreaseData
	lock->m_data<<=1;
	::LeaveCriticalSection(&(lock->m_cr));
	printf("After Increase:%d\n",lock->m_data);

}
void Lock::DecreaseData(Lock * plock)
{
	Lock* lock=plock;
	::EnterCriticalSection(&(lock->m_cr));//注意，DecreaseData和IncreaseData中都要用重要代码段
	plock->m_data>>=1;
	plock->m_data-=1;
	::LeaveCriticalSection(&(lock->m_cr));
	printf("After Decrease:%d\n",lock->m_data);
}
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hdl1=INVALID_HANDLE_VALUE;
	HANDLE hdl2=INVALID_HANDLE_VALUE;
	char c;
	c=getchar();
	fflush(stdin);
	Lock mylock;
	while(c!='q')
	{
		if(c='A')
		{
			hdl1=::CreateThread(0,0,(LPTHREAD_START_ROUTINE)mylock.IncreaseData,&mylock,0,0);
			hdl2=::CreateThread(0,0,(LPTHREAD_START_ROUTINE)mylock.DecreaseData,&mylock,0,0);
			/*
			mylock.IncreaseData(&mylock);  //如果是先后执行的，那么数据会一直不变，但如果是多线程执行，则可能出错
			mylock.DecreaseData(&mylock);
			*/
			Sleep(1000);
		}
		else
			break;
	}
	::CloseHandle(hdl1);
	::CloseHandle(hdl2);
	system("pause");
	return 0;
}

