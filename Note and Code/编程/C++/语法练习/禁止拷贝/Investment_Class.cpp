#include<stdio.h>
#include<stdlib.h>
/*
������Ҫ��ֹһ���౻����ʱ�����Խ���copy���캯����copy assignment ��������Ϊprivate

Effective C++���ᵽ����������
        ��һ����������copy���캯���� copying assignment��������Ϊprivate

        �ڶ������������ǿ�����һ�����࣬������н���copy���캯����copy assignment ��Ϊ˽�У����ǽ�ֹ��������
�����Լ̳�������࣬��ʵ��֤�������������������Ч����Ϊ���������һ��ʲô���ݶ�û�еĻ���ʱ����������
�����Ż�����࣬���������õ�private��copy���캯���������ã�������ᣬ����Ϊ�˱������Ӧ�þ����ܲ��õ�һ������
*/
#include"Investment.h"
#include"CopyFobbid.h"
int main()
{
	Investment Inves1;
	Inves1.SetProperty(1000);
	Investment Inves2(Inves1);
	printf("%d\n",Inves2.GetProperty());

	system("pause");
	return 0;
}