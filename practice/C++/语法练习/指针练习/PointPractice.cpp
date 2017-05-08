//该文件是关于指针的练习
#include<stdio.h>
#include<iostream>

struct memory
{;
	int in[3];
};
typedef struct memory Memory;
int main()
{
	int *p1=nullptr;
	Memory *p2=nullptr;
	Memory *p3=nullptr;
	unsigned char **p4=nullptr;
	unsigned char *p5=nullptr;

	p1=new int[50];
	memset(p1,0,50*sizeof(int));

	p2=new Memory;
	memset(p2,0,sizeof(Memory));

	printf("int* %d\n",sizeof(p1));//打印4，说明地址所占的内存字节数是4
	printf("char %d\n",sizeof(char));//打印1，char占1字节
	printf("int %d\n",sizeof(*p1));//打印4，说明*p1指向的是第一个内存块，占4字节。
	//p1是内存首地址，而首地址指向的，是这个指针类型的字节数，

	printf("Memory* %d\n",sizeof(p2));
	printf("Memory %d\n",sizeof(*p2));

	p3=p2;
	printf("p3,%d\n",sizeof(p3));
	printf("content of p3 %d\n",sizeof(*p3));
	p4=(unsigned char**)p2;
	printf("content of p4 %d\n",sizeof(*p4));
	p5=(unsigned char *)p2;
	printf("content of p5 %d\n",sizeof(*p5));
	//这里我们就可以看出强制转换的效果了。指针的数据类型意味着解释指针的方式，如果我们用unsigned char*p5，那*p5
	//指向了一个unsigned char宽度的内存，如果用unsigned char** p4，那么*p4仍是一个指针，所以占一个指针大小的内存
	//同样Memory* p3，则*p3占了Memory大小的空间
	//我们申请了一个Memory内存大小的空间，假设我们要修改这些内存的其中一部分
	//就可以用另一种类型的指针，比如上例unsigned char *p5去获得Memory的地址，这是当我们操作p5时就只是操作unsigne char 大小的内存内容
	//而不是Memory内存大小的内容，而且可以给这块unsigned char 大小内存进行赋值操作，仅改变这块内存的值
	delete p1;
	delete p2;
	system("pause");
	return 0;
}