/*��ϰָ��ָ���ָ�룬���**p ������*/
#include<stdio.h>
#include<stdlib.h>
/*void main()
{
char str[][25] = { "I like C", "practice make perfect" };
char *p;
p = str;
printf("%s", p);							//�����ӡ��ʱ���õ���p
system("pause");
}*/

/*
{
	int *array[] = { 1, 3, 5, 7 };					//�ȼ���int*array[n],array[n]=&...���ʱ����û�ж������Ԫ�ؽ��и���ַ
	int array2[] = { 2, 4, 6, 8 };					//��ô*array����ָ��δ֪��,�������жϣ�Ҳ����˵�����1��2,3,4����ͨ��array[] *array[]����
	array [0]= array2[0];
	//int **p;
	//p = *array;
	printf("%d\n", array[0]);		//array[0]������ǵ�ַ
	printf("%d\n", *array[0]);
	//printf("%d\n", p);			//����ӡ*p+2�����1+2=3��*(p+2)����5
	//printf("%d\n", **p);
	system("pause");
}
*/

/*
void Trychange1(int *p1,int*p2);
void Trychange2(int *p1,int*p2,int **pt);
int main()
{
	int **pt;
	int *p1,*p2;
	int a,b,c,d;
	p1=&a;
	p2=&b;
	pt=&p1;
	printf("p1 %d, p2  %d\n",p1,p2);
	Trychange1(p1,p2);
	printf("p1 %d, p2  %d\n",p1,p2);
	Trychange2(p1,p2,pt);
	printf("p1 %d, p2  %d\n",p1,p2);
	system("pause");
	return 0;
}
void Trychange1(int *p1,int *p2)
{
	p1=p2;
	printf("p1 %d, p2  %d\n",p1,p2);
}
void Trychange2(int *p1,int *p2,int **pt)
{
	pt=&p2;
	printf("after change 2\np1 %d, p2  %d\n",p1,p2);	//��һ����ӡ������p1��û�б��ı�
														//���ú���������p1���ı���
}
*/
void tryextern();
/*int main()
{
	int a=1;		//a�������򲻰������ļ���main����ĺ���
					//���ڹ۲��������Ƿ��ܹ�ʹ�ò��ı�����������ϰ�ļ����ڲ�ͬ������extern�÷�
	printf("%d\n",a);
	tryextern();
	printf("after function %d\n",a);
	system("pause");
	return 0;
}
extern int a;				extern ��ͬһ���ļ���û�취������������
void tryextern()
{
	a=2;
	printf("%d\n",a);
}*/
/*				����ֻ�����亯���ڲ���ʱ�ı�ָ��ָ���ڴ�ĵ�ַ��ֵ���������ý�����ֵ����ԭ����
void trychange(int *p1,int *p2,int *p3);
int main()
{
	int a,b;
	int *p1=&a;int *p2=&b; int *p3=NULL;
	printf("%d    %d     %d\n",p1,p2,p3);
	trychange(p1,p2,p3);
	printf("after try \n%d    %d    %d\n",p1,p2,p3);
	system("pause");
}

void trychange(int *p1,int *p2,int *p3)
{
	//p1=92,p3=p2;
	p1=(int *)malloc(sizeof (int)),p3=(int *)malloc(sizeof (int)),p2=NULL;
	printf("%d    %d    %d\n",p1,p2,p3);
}*/

/*void trychange(struct student*);
struct student
{
	int a;
	struct student *p;
};
int main()
{
	struct student stu;
	struct student *head=&stu;
	stu.p=(struct student *)malloc(sizeof (struct student));
	stu.a=1;
	printf("head %d  p %d\n",head,stu.p);
	trychange(head);
	printf("after change\nhead %d\n",head);
	system("pause");
}
void trychange(struct student *head)
{
	head=head->p;
	printf("head %d\n",head);
	head->p=(struct student *)malloc(sizeof (struct student));
	head=head->p;
	printf("head %d\n",head);
}*/

/*		//�ṹ������Ƕ�׵��Լ���ָ���ǿ���ͨ�������ı����ֵַ��
		//��������������ӿ�֪����������ӿ�֪�ṹ������ں�����ָ���Լ���ָ��
		//��ô�ں������ý�����õ�ַ���ᱻ�ı�
void trychange(struct student*);
int num;
struct student
{
	int a;
	struct student *p;
};
struct student stu,stu2;
int main()
{
	
	struct student *head=&stu;
	stu.p=(struct student *)malloc(sizeof (struct student));
	stu.a=1;
	printf("%d\n",stu.p);

	trychange(&stu);
	printf("after change\n%d\n",stu.p);

	system("pause");
}
void trychange(struct student *stu)
{
	(*stu).p=&stu2;
	//(*stu).p=(struct student *)malloc(sizeof (struct student));
	printf("%d\n",(*stu).p);
}
*/