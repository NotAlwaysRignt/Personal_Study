#include<stdio.h>
#include<stdlib.h>
/*
当我们要禁止一个类被拷贝时，可以将其copy构造函数和copy assignment 操作符设为private

Effective C++中提到两种做法：
        第一种做法：将copy构造函数和 copying assignment操作符设为private

        第二种做法：我们可以做一个基类，这个类中将其copy构造函数和copy assignment 设为私有，凡是禁止拷贝的类
都可以继承这个基类，但实践证明这个做法并不总是有效，因为当你设计了一个什么内容都没有的基类时，编译器会
尝试优化这个类，导致你设置的private的copy构造函数不起作用，这很讨厌，所以为了保险起见应该尽可能采用第一种做法
*/
#include"Investment.h"
#include"CopyFobbid.h"
int main()
{
	Investment Inves1;
	Inves1.SetProperty(1000);
	Investment Inves2(Inves1);
	printf("%d\n",Inves2.GetProperty());

	system("pause");
	return 0;
}