#include<stdlib.h>
#include<iostream>
using namespace std;
void fun(int *a, int *b)
{
	int c = 0;
	c = *a;
	*a = *b;
	*b = c;
}
/*typedef struct Practice
{
	int x;
	int y;

}Try;*/			//��һ���ַ�Ҫ��д
int main()
{
	int x = 10; int y = 20;
	cout << x <<","<<y<< endl;
	fun(&x, &y);
	cout <<x <<","<<y<< endl;

	system("pause");
}

