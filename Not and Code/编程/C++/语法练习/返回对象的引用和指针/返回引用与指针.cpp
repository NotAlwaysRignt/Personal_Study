/*
���ض�������ú�ָ���������ӵĺ�������Ҫ�ڱ���жž�����ϰ��
*/
#include<stdlib.h>
#include<iostream>
using namespace std;

class Myclass
{
public:
	~Myclass(){
		member=NULL;//�������ã�ָ�룬ֵ���ֵĲ�ͬͨ��member=NULL��һ��������
		cout<<"object was deleted"<<endl;
	}
	void Print () {cout<<"Here is member "<<member<<endl; }
	int member;
};

Myclass& ReturnReffer()
{
	Myclass myclass;
	myclass.member=99;
	return myclass;
}

Myclass* ReturnPoint()
{
	Myclass myclass;
	myclass.member=88;
	return &myclass;
}

const Myclass ReturnValue()
{
	Myclass myclass;
	myclass.member=77;
	return myclass;
}

int main()
{
	cout<<"............................"<<endl;
	Myclass &a=ReturnReffer();
	cout<<a.member<<endl;
	a.Print();
	cout<<"***************"<<endl;

	cout<<"............................"<<endl;
	Myclass *b=ReturnPoint();
	cout<<b->member<<endl;
	b->Print();
	cout<<"***************"<<endl;

	cout<<"............................"<<endl;
	Myclass c=ReturnValue();
	cout<<c.member<<endl;
	c.Print();
	cout<<"***************"<<endl;

	system("pause");
	return 0;
}
/*
�Ӵ�ӡ�Ľ�������������Ǻ�������Refference��Point����Value�������������ᱻ����
�����ڲ�����������ǲ�һ����
          �������������ú�ָ��ʱ�����󽫱����٣�������ķ���ֵ��һ���������˵Ķ���
(��������ʱ��ִ�������������Ҽ�������������û�н��ڲ��ĳ�Ա�������㣬����������
�ĳ�Ա����Ҳ�ᱻ�������ò���֪)
          ����������Valueʱ������ᱻ�ȱ�����һ�ݣ�Ȼ��ԭ���Ķ�������(ִ������������)����
�����ƵĶ��󷵻ء�

ע��һ������:
        ���������е�����������������û��menber=NULL��һ��ʱ���������õ����ص����û�
ָ�룬���ѻ�õ����û�ָ���member��Ա���д�ӡ������member��ֵ��Ȼ���ں����и�����99��88��
����������δ֪�����ݡ�
����Ϊʲô���ѵ�������ʵ��δ�����٣�
        ��ʵ�϶���ȷʵ�Ǳ����ٵ�,�����ڲ�����������Ҳ�ᱻ��ն���ò���֪������ͨ������ִ�е�Print()������
����֪����������������δ�޸�member��ֵ����cout<<a.member<<endl;cout<<b->member<<endl;������ȷʵ���Դ�ӡ��
99��88�������е���һ��a.Printʱ��b->Printʱ��member��ֵ��ʼ�����ˡ��������һ�����ٵ�ʱ��������
�����ǿ��Կ϶���һ��ʱ�������Ƿ��ض����ָ�������ʱ�����ǽ����һ�������ٵĶ�������Բ�����һ��
����ϣ������Ϊ��
�ܽ᣺����Ҫ���ض�������û�ָ��.

*/