// 安全编程和异常处理.cpp : 定义控制台应用程序的入口点。
/*
简单地练习一下(两个冒号)+函数（::function），即全局函数这种语法的作用
在函数前加::,表示这个函数是全局的函数而非类的成员函数，这样即使类中有同名的函数，也不会冲突，而会调用
全局函数，详见下例
*/
#include "stdafx.h"
/*

#include <stdio.h>
#include<string>
using namespace std;
#include<iostream>
class Print
{
public :
	void Printstring()
	{
		printf("I like China");
		::printf("I like China\n");
	}
private:
	int printf(std::string);
};
int Print::printf(std::string  str)
{
	cout<<"print by function of class:"+str<<endl;
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	Print print;
	print.Printstring();
	system("pause");
	return 0;
}
*/
