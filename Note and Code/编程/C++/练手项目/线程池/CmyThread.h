#ifndef CMYTHREAD_H
#define CMYTHREAD_H
#include<windows.h>
//�����������������������̣߳����Զඨ�弸����������Ϊ��֪���������̻߳��м�������
typedef int (*Func)(char*,char,int*,int);
typedef bool (*CallbackFunc)(char*,char,int*,int);


class CmyThread
{

public:
	CmyThread(char*pc1=nullptr,char *pc2=nullptr,int *ip1=nullptr,int*ip2=nullptr,char c1='\0',char c2='\0',
		int i1=0,int i2=0);
	~CmyThread();
	bool IsWorking();//���ö����Ƿ����ڹ���
	bool m_AllowcateJob(Func func,CallbackFunc cfunc,char* pc="This is the result:",char c1='\0',int* pi1=nullptr,int i1=0,
		char* pc2=nullptr,char c2='\0',int*pi2=nullptr,int i2=0);
	//�����������Ƿ�ɹ�,���ɹ���ִ������
private:
	/*�洢Ҫ�����̵߳ĵ�ַ���Լ������*/
	//�洢�����ĵ�ַ
	Func m_func;
	CallbackFunc m_cbFunc;
	//�洢�����Ĳ���
	char * m_pcFun;
	char *m_pcCall;
	int * m_piFun;
	int* m_piCall;
	char m_cFun;
	char m_cCall;
	int m_iFun;
	int m_iCall;
	bool m_bworking;//����������ҪС�ģ�������Ҫ����ʱҪ��������ؿ콫����true����Ϊ�ڸ��ٵ��������������Ѿ���ʼִ��
	//��m_bworkingȴû����true�����Ǻ�Σ�յģ�����ʵ������Ȼ�����ƣ���Ϊ���������߳����ж�ʱ����������false(ͬʱ�ڽ�����falseǰ�ж���)��
	//��������ǵ�һ���߳��жϵ�����δ�����ͼӰ���,����������true,���������߳���ʹ����

	
	HANDLE m_HThread;//Ҫ�����Լ����̣߳�����Ҫ���߳̾��
	
	void m_jobDone();//ִ��������,��ʱ���̹߳���
	
	/*�ú�����ΪCreateThread�Ĳ��������벢��������߳���ռ��λ*/
     static int  FuncProc(int * const CLASS);//��ʱ����������Ϊ��������������������ڸú����в����Լ��ĳ�Ա
	 //����static �޷���CreateThread��ʵ��(LPTHREAD_START_ROUTINE)��ת������Ϊ�̺߳��������Ǿ�̬�Ļ�ȫ�ֵ�
};

#endif