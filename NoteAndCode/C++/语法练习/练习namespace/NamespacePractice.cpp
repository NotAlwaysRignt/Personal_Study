#include<iostream>
#include<stdlib.h>
using namespace std;		//如果没有这句那么几就要在cout和endl前都加上std::

namespace A
{
	int x = 1;
	void fun()
	{
		cout << "A" << endl;
	}
	
}
namespace B
{
	int x = 2;
	void fun()
	{
		cout << "B" << endl;
	}
	void fun2()
	{
		cout << "2B" << endl;
	}
}
						//using namespace A;如果加上这一句会报错，因为fun()重复了
using namespace B;		//这句话应该放在创建命名空间之后，顺序的错误会导致无法引用
int main()
{
	cout << A::x << endl;
	fun();
	system("pause");
	return 0;

}