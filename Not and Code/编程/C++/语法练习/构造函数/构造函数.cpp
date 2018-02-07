#include<stdlib.h>
class A
{
public:
	A(int a,int b){}

};

class B
{
public:
	B():_classA(1,2){}
private:
	A _classA;//注意这里并没有传给A的构造函数参数int a和int b
};
int main()
{
	//A _classA;  //error，没有合适的构造函数，因为A得构造函数要求两个int的参数
	B _classB; //可以通过
	system("pause");
    return 0;
}

/*
在这里可以看出类成员函数的生成时机是在类的构造函数执行时
以上面为例，B的成员函数为 A _classA;如果A _classA不是放在B中定义，而是放在main函数中，
则不会通过编译，因为A的构造函数要求两个int类型的参数，但这里放在B中是可以的，
因为B的构造函数中有对A的初始化，即_classA(1,2)，虽然下面 private:A _classA; 看似不合法
这里我们可以看出类对象的生成时机，即在类的构造函数执行时生成
并不是在 private: A _classA; 此处就开始生成
*/