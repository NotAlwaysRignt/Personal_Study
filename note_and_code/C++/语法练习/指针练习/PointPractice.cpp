//���ļ��ǹ���ָ�����ϰ
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

	printf("int* %d\n",sizeof(p1));//��ӡ4��˵����ַ��ռ���ڴ��ֽ�����4
	printf("char %d\n",sizeof(char));//��ӡ1��charռ1�ֽ�
	printf("int %d\n",sizeof(*p1));//��ӡ4��˵��*p1ָ����ǵ�һ���ڴ�飬ռ4�ֽڡ�
	//p1���ڴ��׵�ַ�����׵�ַָ��ģ������ָ�����͵��ֽ�����

	printf("Memory* %d\n",sizeof(p2));
	printf("Memory %d\n",sizeof(*p2));

	p3=p2;
	printf("p3,%d\n",sizeof(p3));
	printf("content of p3 %d\n",sizeof(*p3));
	p4=(unsigned char**)p2;
	printf("content of p4 %d\n",sizeof(*p4));
	p5=(unsigned char *)p2;
	printf("content of p5 %d\n",sizeof(*p5));
	//�������ǾͿ��Կ���ǿ��ת����Ч���ˡ�ָ�������������ζ�Ž���ָ��ķ�ʽ�����������unsigned char*p5����*p5
	//ָ����һ��unsigned char��ȵ��ڴ棬�����unsigned char** p4����ô*p4����һ��ָ�룬����ռһ��ָ���С���ڴ�
	//ͬ��Memory* p3����*p3ռ��Memory��С�Ŀռ�
	//����������һ��Memory�ڴ��С�Ŀռ䣬��������Ҫ�޸���Щ�ڴ������һ����
	//�Ϳ�������һ�����͵�ָ�룬��������unsigned char *p5ȥ���Memory�ĵ�ַ�����ǵ����ǲ���p5ʱ��ֻ�ǲ���unsigne char ��С���ڴ�����
	//������Memory�ڴ��С�����ݣ����ҿ��Ը����unsigned char ��С�ڴ���и�ֵ���������ı�����ڴ��ֵ
	delete p1;
	delete p2;
	system("pause");
	return 0;
}