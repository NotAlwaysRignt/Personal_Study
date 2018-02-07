/*
���ĸ���A B C D������D�̳�C��C�̳�B��B�̳�A�����Ƕ������صĺ���f()��
��Ҫ�ø����ָ�������������f()��Ҫ���������(����֮ǰ�ĸ���)��f()����Ϊ
virtual������A��f()�����麯����B�е�f()���麯�������ж���D����A��ָ��ָ������
A�޷�����D��f()����B��ָ��ָ��D��B���Ե���D��f() 
�������صĺ�������һ�������һ�ε��� virtual����������麯����ʼ���麯�����и���

�麯��������:ʵ���ø�������������ķ���
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
