#ifndef _TEST_CLASS
#define _TEST_CLASS
/*
����ģ����A B C  ����C�̳�B��B�̳�A ��A�̳���Base(Base������ģ����)
����Super��̳У�A B C���ں�������ʵ�飬Super��Base����һ��ʵ��typename��ʹ��ʵ��
*/

class Base
{
public:
	void Print(){
		printf("Base Class");
		}
};

template<typename T>
class A:public Base
{
public :
	void Print() const{
		printf("A\n");
	}
/*A�ж�����һ��Base��*/	

};

template<typename T>
class B:public A<T>
{
	public :
		void Print() const{
			typename A<T>::Print(); 
			printf("B\n");
		}
};

template<typename T>
class C:public B<T>
{
	public :
		void Print() const{
			 typename B<T>::Print();	
			printf("C\n");}
};


template<typename T>
class Super:public A<T>::Base
{
public :
	void Print()
	{
		typename A<T>::Base b;
		b.Print();
	}

};


#endif