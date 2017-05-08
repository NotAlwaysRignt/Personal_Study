#include<stdlib.h>
#include<stdio.h>
#include"my_test_class.h"
/*三个重载函数*/
template<typename T>
void func(const C<T>& c)
{
	c.Print();
}

template<typename T>
void func(const B<T>& b) //注意传入的是const reference，所以只能调用常函数
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
	/*实验证明，虽然C public继承了B，B public继承了A，即C和 A是is-a的关系，
	理论上func(const A<T>& a)一样会接收C，但当传给func的类型为C时编译器会选择最合适的重载函数即
	传给func(const C<T>& c),当传给func的类型为C而没有func(const C<T>& c)而只有func(const A<T>& a)和
	func(const B<T>& b)时，编译器会选择将C传给func(const B<T>& b)，始终选择"最大化匹配" */
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
