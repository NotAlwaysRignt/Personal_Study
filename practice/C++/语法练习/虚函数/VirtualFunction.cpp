/*
设四个类A B C D，其中D继承C，C继承B，B继承A，他们都有重载的函数f()，
则要用父类的指针来调用子类的f()，要求这个父类(或父类之前的父类)将f()声明为
virtual。例如A的f()不是虚函数，B中的f()是虚函数，现有对象D，用A的指针指向它，
A无法调用D的f()，用B的指针指向D，B可以调用D的f() 
对于重载的函数，在一个基类第一次调用 virtual后其子类的虚函数开始在虚函数表中覆盖

虚函数的作用:实现用父类来调用子类的方法
*/
#include<iostream>
#include<stdlib.h>
using namespace std;
namespace VirtualPractice{
	class Person
{
public:
	Person(){cout<<"Person is created"<<endl;}
    void DoSomething(){cout<<"This is Person"<<endl; ProFun();}

protected:
	virtual void ProFun(){cout<<"This is protected member in Person\n"<<endl;}
	
};

	class Worker:public Person
{
public:
	Worker(){cout<<"Worker is created"<<endl;}
	void DoSomething(){cout<<"This is Worker"<<endl; ProFun();}

protected:
	void ProFun(){cout<<"This is protected member in Worker\n"<<endl;  }
};

	class Engineer:public Worker
{
public:
	Engineer(){cout<<"Engineer is created"<<endl;}
    virtual void DoSomething(){cout<<"This is Engineer"<<endl;ProFun();}

protected:
	void ProFun(){cout<<"This is protected member in Engineer\n"<<endl;}
};

class Boss:public Engineer
{
public:
	Boss(){cout<<"Boss is created"<<endl;}
     void DoSomething(){cout<<"This is Boss"<<endl;ProFun();}

protected:
	void ProFun(){cout<<"This is protected member in Boss\n"<<endl;}
};
}
int main()
{
	VirtualPractice::Boss Cengineer;
	VirtualPractice::Person* pperson=&Cengineer;	
	VirtualPractice::Worker* pworker=&Cengineer;
	VirtualPractice::Engineer* pengineer=&Cengineer;
	VirtualPractice::Boss *pboss=&Cengineer;

	pperson->DoSomething();
	pworker->DoSomething();
	pengineer->DoSomething();
	pboss->DoSomething();

	system("pause");
	return 0;
}
