#pragma once  //�������pragma once����Ҫ��#ifndef... def...(ͷ�ļ�����) #endif����ʽ
#include"CmyThread.h"
#include<vector>
using std::vector;
class CmyPool
{
public:
	CmyPool(int n);
	~CmyPool(void);
	unsigned int  getsize();
	int dojob(Func,CallbackFunc);

private:
	vector<CmyThread*>  m_Threads;

};

