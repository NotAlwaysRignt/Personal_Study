#include<stdlib.h>
#include<stdio.h>
#include"my_test_class.h"
/*�������غ���*/
template<typename T>
void func(const C<T>& c)
{
	c.Print();
}

template<typename T>
void func(const B<T>& b) //ע�⴫�����const reference������ֻ�ܵ��ó�����
{
	b.Print();
}

template<typename T>
void func(const A<T>& a)
{
	a.Print();
}

int main()
{
	/*ʵ��֤������ȻC public�̳���B��B public�̳���A����C�� A��is-a�Ĺ�ϵ��
	������func(const A<T>& a)һ�������C����������func������ΪCʱ��������ѡ������ʵ����غ�����
	����func(const C<T>& c),������func������ΪC��û��func(const C<T>& c)��ֻ��func(const A<T>& a)��
	func(const B<T>& b)ʱ����������ѡ��C����func(const B<T>& b)��ʼ��ѡ��"���ƥ��" */
	A<int> a;
	C<int> c;
	func(a);
	printf("/*****************/\n");
	func(c);
	printf("/*****************/\n");
	Super<int> s;
	s.Print();
	system("pause");
    return 0;
}
