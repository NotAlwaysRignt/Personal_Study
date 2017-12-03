// ����.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
/*ѧ��ʹ��CriticalSection(�ؼ������)���ܷ�ֹ��������*/
//��Ҫ�õ��ĺ���
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
	Sleep(1);//�����ִ����lock->m_data+=1;ʱplock->m_data>>=1;��ִ���ˣ���ô���ݾͻ����;
	//��ʹȥ��Sleep(),���ִ���Ҳ�ᷢ�����������Ϊ�˷�ֹ���ִ��������ƺ�ʱ��Ƭ������֤��ִ����IncreaseData��ִ��DecreaseData
	lock->m_data<<=1;
	::LeaveCriticalSection(&(lock->m_cr));
	printf("After Increase:%d\n",lock->m_data);

}
void Lock::DecreaseData(Lock * plock)
{
	Lock* lock=plock;
	::EnterCriticalSection(&(lock->m_cr));//ע�⣬DecreaseData��IncreaseData�ж�Ҫ����Ҫ�����
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
			mylock.IncreaseData(&mylock);  //������Ⱥ�ִ�еģ���ô���ݻ�һֱ���䣬������Ƕ��߳�ִ�У�����ܳ���
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

