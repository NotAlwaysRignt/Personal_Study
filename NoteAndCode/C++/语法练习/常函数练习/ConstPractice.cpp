/*#include"ConstPractice.h"
#include<stdlib.h>
#include<iostream>

const  int MyClass::stanum=5;
const int MyClass::m_connum=8;
const int MyClass::Confunc() const
{
	//const int m_connum=20;
	std::cout<<m_connum<<std::endl;
	std::cout<<stanum<<std::endl;
	std::cout<<"Success"<<std::endl;
	return 0;
}

int MyClass::func()
{
	//const int m_connum=15;//正确 可以在非常函数中初始化常成员，前提是常成员在声明时未初始化
	std::cout<<m_connum<<std::endl;
	std::cout<<"Yeah"<<std::endl;
	return 0;
}

void MyClass::print()
{
	//const int m_connum=10;
	std::cout<<m_connum<<std::endl;
}
int main()
{
	const MyClass class1;
	MyClass class2;
	class1.Confunc();
    //class1.func(); 错误常对象不能调用非常函数
	class2.Confunc();
	class2.func();
	
	//class2.print();
	system("pause");
	return 0;
}*/