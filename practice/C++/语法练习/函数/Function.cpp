#include<stdlib.h>
#include<iostream>
using namespace std;

inline void fun(int i=30, int j=20, int k=10);
inline void fun(double i, double j);
int main(void)
{
	//fun();
	//fun(100);
	//fun(100, 200);
	//fun(100, 200, 300);
	fun(1.1, 2.2);
	fun(1, 2);
	system("pause");
	return 0;
}
void fun(int a,int b,int c)
{
	cout << a <<"," <<b<<"," << c<<endl;
}
void fun(double i, double j)
{
	cout << i <<","<< j << endl;
}