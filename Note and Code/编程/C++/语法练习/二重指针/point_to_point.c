/*练习指向指针的指针，理解**p 的意义*/
#include<stdio.h>
#include<stdlib.h>
/*void main()
{
char str[][25] = { "I like C", "practice make perfect" };
char *p;
p = str;
printf("%s", p);							//这里打印的时候用的是p
system("pause");
}*/

/*
{
	int *array[] = { 1, 3, 5, 7 };					//等价与int*array[n],array[n]=&...这个时候若没有对数组的元素进行赋地址
	int array2[] = { 2, 4, 6, 8 };					//那么*array将是指向未知的,会引发中断，也就是说里面的1，2,3,4不能通过array[] *array[]读到
	array [0]= array2[0];
	//int **p;
	//p = *array;
	printf("%d\n", array[0]);		//array[0]里面的是地址
	printf("%d\n", *array[0]);
	//printf("%d\n", p);			//若打印*p+2则会是1+2=3；*(p+2)才是5
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
	printf("after change 2\np1 %d, p2  %d\n",p1,p2);	//这一步打印出来的p1并没有被改变
														//而该函数结束后p1被改变了
}
*/
void tryextern();
/*int main()
{
	int a=1;		//a的作用域不包括该文件的main以外的函数
					//现在观察声明后是否能够使用并改变它，这是练习文件以内不同函数的extern用法
	printf("%d\n",a);
	tryextern();
	printf("after function %d\n",a);
	system("pause");
	return 0;
}
extern int a;				extern 在同一个文件中没办法起到声明的作用
void tryextern()
{
	a=2;
	printf("%d\n",a);
}*/
/*				函数只能在其函数内部临时改变指针指向内存的地址的值，函数调用结束后值会变回原来的
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

/*		//结构体里面嵌套的自己的指针是可以通过函数改变其地址值的
		//但是由上面的例子可知由上面的例子可知结构体如果在函数中指向自己的指针
		//那么在函数调用结束后该地址不会被改变
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