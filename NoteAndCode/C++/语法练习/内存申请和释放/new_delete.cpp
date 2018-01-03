#include<stdlib.h>
#include<iostream>
using namespace std;

int main()
{
	int *p = new int[5];
	if (NULL == p)
	{
		system("pause");
		return 0;
	}
	p[0] = 1; p[1] = 2;
	cout << *p << ","<<*(p+1)<<endl;
	delete []p;
	p = NULL;
	system("pause");
	return 0;
}