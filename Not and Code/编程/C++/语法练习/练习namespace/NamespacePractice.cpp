#include<iostream>
#include<stdlib.h>
using namespace std;		//���û�������ô����Ҫ��cout��endlǰ������std::

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
						//using namespace A;���������һ��ᱨ����Ϊfun()�ظ���
using namespace B;		//��仰Ӧ�÷��ڴ��������ռ�֮��˳��Ĵ���ᵼ���޷�����
int main()
{
	cout << A::x << endl;
	fun();
	system("pause");
	return 0;

}