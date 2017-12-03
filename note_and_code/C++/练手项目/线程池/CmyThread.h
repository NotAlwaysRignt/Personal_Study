#ifndef CMYTHREAD_H
#define CMYTHREAD_H
#include<windows.h>
//定义两个函数，用来接收线程，可以多定义几个参数，因为不知道传来的线程会有几个参数
typedef int (*Func)(char*,char,int*,int);
typedef bool (*CallbackFunc)(char*,char,int*,int);


class CmyThread
{

public:
	CmyThread(char*pc1=nullptr,char *pc2=nullptr,int *ip1=nullptr,int*ip2=nullptr,char c1='\0',char c2='\0',
		int i1=0,int i2=0);
	~CmyThread();
	bool IsWorking();//检查该对象是否正在工作
	bool m_AllowcateJob(Func func,CallbackFunc cfunc,char* pc="This is the result:",char c1='\0',int* pi1=nullptr,int i1=0,
		char* pc2=nullptr,char c2='\0',int*pi2=nullptr,int i2=0);
	//检查分配任务是否成功,若成功就执行任务
private:
	/*存储要接收线程的地址，以及其参数*/
	//存储函数的地址
	Func m_func;
	CallbackFunc m_cbFunc;
	//存储函数的参数
	char * m_pcFun;
	char *m_pcCall;
	int * m_piFun;
	int* m_piCall;
	char m_cFun;
	char m_cCall;
	int m_iFun;
	int m_iCall;
	bool m_bworking;//处理它必须要小心，当对象要工作时要尽可能早地快将它置true，因为在高速的情况下如果任务已经开始执行
	//但m_bworking却没有置true，那是很危险的，但事实上这仍然不完善，因为可能两个线程在判断时都读到它是false(同时在将其置false前判断它)，
	//所以最好是当一个线程判断到对象未工作就加把锁,紧接着再置true,不让其它线程再使用它

	
	HANDLE m_HThread;//要操纵自己的线程，所以要有线程句柄
	
	void m_jobDone();//执行完任务,暂时将线程挂起
	
	/*该函数作为CreateThread的参数，申请并挂起这个线程来占好位*/
     static int  FuncProc(int * const CLASS);//到时把整个类作为参数传进来，这样便可在该函数中操作自己的成员
	 //不用static 无法在CreateThread里实现(LPTHREAD_START_ROUTINE)的转换，因为线程函数必须是静态的或全局的
};

#endif