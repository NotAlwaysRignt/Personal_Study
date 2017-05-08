#include"stdafx.h"
#include"CmyThread.h"
#include<stdio.h>
#include<windows.h>

CmyThread::CmyThread(char *pc1, char*pc2, int* pi1, int*pi2, char c1, char c2, int i1, int i2)
  : m_pcFun(pc1), 
	m_pcCall(pc2),
	m_piFun(pi1),
	m_piCall(pi2),
	m_cFun(c1),
	m_cCall(c2),
	m_iFun(i1),
	m_iCall(i2)
{
	m_func=0;
	m_cbFunc=0;//记得初始化func和cbfunc，否则在CreateThread中非0值得func和cbfunc将被执行
	m_bworking=false;
	m_HThread=NULL;
	m_HThread =CreateThread(0,0,(LPTHREAD_START_ROUTINE)FuncProc,this,0,0);
	//线程函数要求是静态成员函数或全局函数，因此FuncProc要求是static的
	if (m_HThread==NULL)
	{
		printf("fail to create thread");
	}
	printf("Create New Thread %08x\n",m_HThread);
}

CmyThread::~CmyThread()
{
	if(m_HThread!=INVALID_HANDLE_VALUE)//说明线程并未关闭
	{
		m_func=(Func)INVALID_HANDLE_VALUE;
		m_cbFunc=(CallbackFunc)INVALID_HANDLE_VALUE;
		ResumeThread(m_HThread);
		//先执行子类的析构函数再执行父类的析构函数
		WaitForSingleObject(m_HThread, INFINITE);
		CloseHandle(m_HThread);
	}
}

bool CmyThread::IsWorking()
{
	return this->m_bworking;
}

void CmyThread::m_jobDone()
{
	m_bworking=false;
	SuspendThread(m_HThread);
}

bool CmyThread::m_AllowcateJob
       (Func func,CallbackFunc cbfunc,char *pc1,char c1,int* pi1,int i1,char* pc2,char c2,int* pi2,int i2)
{
	m_pcFun=pc1;
	m_cFun=c1;
	m_piFun=pi1;
	m_iFun=i1;
	m_pcCall=pc2;
	m_cCall=c2;
	m_piCall=pi2;
	m_iCall=i2;

	if(func==nullptr||cbfunc==nullptr)
		return false;
	if(func==INVALID_HANDLE_VALUE||cbfunc==INVALID_HANDLE_VALUE)
	{
		printf("Fail to allowcate job\n");
		m_bworking=false;//因为开始使用前m_bworking置了true，发现函数使用不了要将m_bworking置false，才能在下次再使用
		return false;
	}

    m_func=func;
	m_cbFunc=cbfunc;
	ResumeThread(m_HThread);
	printf("thread %08X start work\n", m_HThread);
	return true;
}
int CmyThread::FuncProc(int  *const  Cthread)
{//一个细节: 构造函数用了CreateThread，如果代码中还没给这个函数写内容( 即没有int CmyThread::FuncProc(int  *const  Cthread){...}这部分 )
	//则会报错说FuncProc这个函数时无法解析的外部符号，让人很难理解，难以调试
	CmyThread * cmythread=(CmyThread *)Cthread;//在static函数内，我们没办法直接使用类的成员，只能作参数传入
	while(true)//在这里不断循环，每做完一次工作就挂起（最后一句），有工作了就执行一次循环里的过程（从第一句开始）
	{
		if(cmythread->m_func==nullptr||cmythread->m_cbFunc==nullptr)//线程分配失败就暂停(挂起)不让它执行，下次恢复再检查，直到分配成功
			cmythread->m_jobDone();


		if(cmythread->m_func==INVALID_HANDLE_VALUE||cmythread->m_cbFunc==INVALID_HANDLE_VALUE)
		{
			printf("thread %08X byebye\n",cmythread->m_HThread);
			break;
		}//这一步是在CmyThread执行析构函数时，因为在子类里m_func已经被清空，所以不能再执行下面的代码
		cmythread->m_bworking=true;
		cmythread->m_func(cmythread->m_pcFun,cmythread->m_cFun,cmythread->m_piFun,cmythread->m_iFun);
		cmythread->m_cbFunc(cmythread->m_pcCall,cmythread->m_cCall,cmythread->m_piCall,cmythread->m_iCall);
		cmythread->m_jobDone();		  
	}
	return 0;
}
