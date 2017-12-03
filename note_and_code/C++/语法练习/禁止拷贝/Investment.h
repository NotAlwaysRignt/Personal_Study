#pragma once
#include"CopyFobbid.h"
class Investment//:private Fobbiden
{
public:
	Investment(){};
	/*Investment(const Investment& rhs):
	  m_property(rhs.m_property)
	 {}
	  Investment& operator=(const Investment& rhs)
	 {
		 m_property=rhs.m_property;
		 return *this;
	  }*/
	  void SetProperty(int pro){	m_property=pro;}
	  int GetProperty(){	return m_property;}
private:
	Investment(const Investment&){}//copy构造函数和copying assignment操作符都要放在private
	Investment& operator=(const Investment&){}
	//实践证明当你把copy构造函数放在public而operator=放在私有时，Investment v1(v2) Investment v1=v2依然可能会起作用
	//因此当你不想这个类被拷贝，最保险的方法是copy构造函数和copying assignment操作符都要放在private
	int m_property;
};