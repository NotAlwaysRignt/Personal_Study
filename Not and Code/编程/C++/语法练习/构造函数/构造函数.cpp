#include<stdlib.h>
class A
{
public:
	A(int a,int b){}

};

class B
{
public:
	B():_classA(1,2){}
private:
	A _classA;//ע�����ﲢû�д���A�Ĺ��캯������int a��int b
};
int main()
{
	//A _classA;  //error��û�к��ʵĹ��캯������ΪA�ù��캯��Ҫ������int�Ĳ���
	B _classB; //����ͨ��
	system("pause");
    return 0;
}

/*
��������Կ������Ա����������ʱ��������Ĺ��캯��ִ��ʱ
������Ϊ����B�ĳ�Ա����Ϊ A _classA;���A _classA���Ƿ���B�ж��壬���Ƿ���main�����У�
�򲻻�ͨ�����룬��ΪA�Ĺ��캯��Ҫ������int���͵Ĳ��������������B���ǿ��Եģ�
��ΪB�Ĺ��캯�����ж�A�ĳ�ʼ������_classA(1,2)����Ȼ���� private:A _classA; ���Ʋ��Ϸ�
�������ǿ��Կ�������������ʱ����������Ĺ��캯��ִ��ʱ����
�������� private: A _classA; �˴��Ϳ�ʼ����
*/