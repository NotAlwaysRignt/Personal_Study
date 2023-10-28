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
	m_cbFunc=0;//�ǵó�ʼ��func��cbfunc��������CreateThread�з�0ֵ��func��cbfunc����ִ��
	m_bworking=false;
	m_HThread=NULL;
	m_HThread =CreateThread(0,0,(LPTHREAD_START_ROUTINE)FuncProc,this,0,0);
	//�̺߳���Ҫ���Ǿ�̬��Ա������ȫ�ֺ��������FuncProcҪ����static��
	if (m_HThread==NULL)
	{
		printf("fail to create thread");
	}
	printf("Create New Thread %08x\n",m_HThread);
}

CmyThread::~CmyThread()
{
	if(m_HThread!=INVALID_HANDLE_VALUE)//˵���̲߳�δ�ر�
	{
		m_func=(Func)INVALID_HANDLE_VALUE;
		m_cbFunc=(CallbackFunc)INVALID_HANDLE_VALUE;
		ResumeThread(m_HThread);
		//��ִ�����������������ִ�и������������
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
		m_bworking=false;//��Ϊ��ʼʹ��ǰm_bworking����true�����ֺ���ʹ�ò���Ҫ��m_bworking��false���������´���ʹ��
		return false;
	}

    m_func=func;
	m_cbFunc=cbfunc;
	ResumeThread(m_HThread);
	printf("thread %08X start work\n", m_HThread);
	return true;
}
int CmyThread::FuncProc(int  *const  Cthread)
{//һ��ϸ��: ���캯������CreateThread����������л�û���������д����( ��û��int CmyThread::FuncProc(int  *const  Cthread){...}�ⲿ�� )
	//��ᱨ��˵FuncProc�������ʱ�޷��������ⲿ���ţ����˺�����⣬���Ե���
	CmyThread * cmythread=(CmyThread *)Cthread;//��static�����ڣ�����û�취ֱ��ʹ����ĳ�Ա��ֻ������������
	while(true)//�����ﲻ��ѭ����ÿ����һ�ι����͹������һ�䣩���й����˾�ִ��һ��ѭ����Ĺ��̣��ӵ�һ�俪ʼ��
	{
		if(cmythread->m_func==nullptr||cmythread->m_cbFunc==nullptr)//�̷߳���ʧ�ܾ���ͣ(����)������ִ�У��´λָ��ټ�飬ֱ������ɹ�
			cmythread->m_jobDone();


		if(cmythread->m_func==INVALID_HANDLE_VALUE||cmythread->m_cbFunc==INVALID_HANDLE_VALUE)
		{
			printf("thread %08X byebye\n",cmythread->m_HThread);
			break;
		}//��һ������CmyThreadִ����������ʱ����Ϊ��������m_func�Ѿ�����գ����Բ�����ִ������Ĵ���
		cmythread->m_bworking=true;
		cmythread->m_func(cmythread->m_pcFun,cmythread->m_cFun,cmythread->m_piFun,cmythread->m_iFun);
		cmythread->m_cbFunc(cmythread->m_pcCall,cmythread->m_cCall,cmythread->m_piCall,cmythread->m_iCall);
		cmythread->m_jobDone();		  
	}
	return 0;
}
