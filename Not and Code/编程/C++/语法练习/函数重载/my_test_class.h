#ifndef _TEST_CLASS
#define _TEST_CLASS
/*
三个模板类A B C  其中C继承B，B继承A ，A继承了Base(Base并不是模板类)
类由Super类继承，A B C用于函数重载实验，Super和Base供另一组实验typename的使用实验
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
/*A中定义了一个Base类*/	

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