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
	Investment(const Investment&){}//copy���캯����copying assignment��������Ҫ����private
	Investment& operator=(const Investment&){}
	//ʵ��֤�������copy���캯������public��operator=����˽��ʱ��Investment v1(v2) Investment v1=v2��Ȼ���ܻ�������
	//��˵��㲻������౻��������յķ�����copy���캯����copying assignment��������Ҫ����private
	int m_property;
};