/*����ϰ����ָ��ǰ����ϰnew���÷�*/
#include<iostream>
#include<stdlib.h>
#include<memory>
using namespace std;
class Library{
public:
	Library(int n=10):books(n){cout<<"A new object was created"<<endl;}
	~Library()
	{	cout<<"This object was deleted"<<endl;}
	int GetBookNum(){return books;}
	void SetBookNum(int n){books =n;}
private:
	int books;
};
int  TestFun()
{	
	shared_ptr<Library> p1=make_shared<Library>(5);
    //�ȼ���shared_ptr<Library> p1(new Library(5)); 
	//shared_ptr<Library> p1= new Library(5)�ǲ���ת����
	//��shared_ptr����һ����ȥ�ü���
	cout<<"There are " <<p1->GetBookNum()<<" books(p1)"<<endl;
	shared_ptr <Library> p2=p1;
	p2->SetBookNum(20);
	cout<<"Now there are " <<p1->GetBookNum()<<" books(p1)"<<endl;
	cout<<"Now there are " <<p2->GetBookNum()<<" books(p2)"<<endl;
	return 0;
}
int main(){
	TestFun();
	system("pause");
	return 0;
}