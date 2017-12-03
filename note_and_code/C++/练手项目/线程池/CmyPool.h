#pragma once  //如果不用pragma once，就要用#ifndef... def...(头文件内容) #endif的形式
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

