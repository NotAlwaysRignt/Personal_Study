#include "StdAfx.h"
#include "CmyPool.h"
const int Success =1;

CmyPool::CmyPool(int n)
{
	if(n<=0)
	{
		throw "Wrong Parameter";
	}
	for(int i=0;i<n;i++)
	{
		CmyThread* tmp=new CmyThread();
		m_Threads.push_back(tmp);
	}
}


CmyPool::~CmyPool(void)
{
	vector<CmyThread*>::iterator iter=m_Threads.begin();
	for(;iter!=m_Threads.end();)
	{
		CmyThread *tmp=*iter;
		iter++;//两步可简写为一步CmyThread* tmp=*iter++;
		delete tmp;
	}
}

int CmyPool::dojob(Func func ,CallbackFunc cbfunc)
{
	vector<CmyThread*>::const_iterator iter=m_Threads.begin();
	for(;iter!=m_Threads.end();iter++)
	{
		if(!(*iter)->IsWorking())
		{
			(*iter)->m_AllowcateJob(func,cbfunc);
			return Success;
		}
	}
	CmyThread * tmp=new CmyThread();
	m_Threads.push_back(tmp);
	tmp->m_AllowcateJob(func,cbfunc);
	return Success;
}

unsigned int  CmyPool::getsize()
{
	return m_Threads.size();
}