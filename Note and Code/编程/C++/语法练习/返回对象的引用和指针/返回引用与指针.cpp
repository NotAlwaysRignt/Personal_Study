/*
返回对象的引用和指针会产生恶劣的后果，因此要在编程中杜绝这种习惯
*/
#include<stdlib.h>
#include<iostream>
using namespace std;

class Myclass
{
public:
	~Myclass(){
		member=NULL;//返回引用，指针，值表现的不同通过member=NULL这一步来体现
		cout<<"object was deleted"<<endl;
	}
	void Print () {cout<<"Here is member "<<member<<endl; }
	int member;
};

Myclass& ReturnReffer()
{
	Myclass myclass;
	myclass.member=99;
	return myclass;
}

Myclass* ReturnPoint()
{
	Myclass myclass;
	myclass.member=88;
	return &myclass;
}

const Myclass ReturnValue()
{
	Myclass myclass;
	myclass.member=77;
	return myclass;
}

int main()
{
	cout<<"............................"<<endl;
	Myclass &a=ReturnReffer();
	cout<<a.member<<endl;
	a.Print();
	cout<<"***************"<<endl;

	cout<<"............................"<<endl;
	Myclass *b=ReturnPoint();
	cout<<b->member<<endl;
	b->Print();
	cout<<"***************"<<endl;

	cout<<"............................"<<endl;
	Myclass c=ReturnValue();
	cout<<c.member<<endl;
	c.Print();
	cout<<"***************"<<endl;

	system("pause");
	return 0;
}
/*
从打印的结果来看，不管是函数返回Refference、Point还是Value，析构函数都会被调用
但是内部发生的情况是不一样的
          当函数返回引用和指针时，对象将被销毁，最后函数的返回值是一个被销毁了的对象
(对象被销毁时会执行析构函数，且即便析构函数并没有将内部的成员变量置零，最后这个对象
的成员变量也会被清理而变得不可知)
          当函数返回Value时，对象会被先被复制一份，然后原来的对象被销毁(执行其析构函数)，并
将复制的对象返回。

注意一个现象:
        上述例子中当我们在析构函数中没有menber=NULL这一步时，当我们拿到返回的引用或
指针，并把获得的引用或指针的member成员进行打印，发现member的值仍然是在函数中赋给的99或88。
而不是其他未知的数据。
这是为什么？难道对象其实并未被销毁？
        事实上对象确实是被销毁的,对象内部的数据最终也会被清空而变得不可知，我们通过后来执行的Print()函数中
可以知道，当析构函数中未修改member的值，在cout<<a.member<<endl;cout<<b->member<<endl;中我们确实可以打印出
99或88，但进行到下一步a.Print时，b->Print时，member的值开始变乱了。这或许是一个销毁的时机的问题
但我们可以肯定的一点时，当我们返回对象的指针或引用时，我们将获得一个被销毁的对象，这绝对不会是一个
我们希望的行为。
总结：绝不要返回对象的引用或指针.

*/