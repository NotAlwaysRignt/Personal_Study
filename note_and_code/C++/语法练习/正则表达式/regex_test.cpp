#include<regex>
#include<stdlib.h>
#include<iostream>
/*typedef match_results<string::const_iterator> smatch;
��smatch�����ı�����match_results<string::const_iterator>
smach��������ƥ��Ľ��

regex_match(string�࣬regex��)���ص���bool���ͣ�����ɹ�ƥ���򷵻�True����ƥ��ʧ���򷵻�False
regex_search(string�࣬regex��)���ص���bool���ͣ�����ɹ�ƥ���򷵻�True����ƥ��ʧ���򷵻�False
regex_match��ȫ��ƥ�䣬��regex_search����������ƥ����ַ���
regex_replace���滻������ʽƥ�����ݵĺ�������ϸ���ĵ���C++primer
*/
	int main(int argv,char *argc)
{
	std::smatch result;
	std::string str("Test.txt  Hello.txt  Goodbye.txt");
	std::regex reg("[A-Za-z]+\.txt");
	//std::cout<<std::regex_match(str,reg)<<std::endl;
	std::cout<<std::regex_search(str,result,reg)<<std::endl;
	for (std::sregex_iterator it(str.begin(),str.end(),reg),  end_it;    it!=end_it;    it++)
		//end_it�Զ���ı���������sregex_iterator����,����end_itΪ��,��Ч�ڵ�������β��(������end())
	    std::cout<< it->str() <<std::endl;
	system("pause");
	return 0;
}