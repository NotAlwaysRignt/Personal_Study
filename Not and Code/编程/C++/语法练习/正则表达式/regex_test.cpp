#include<regex>
#include<stdlib.h>
#include<iostream>
/*typedef match_results<string::const_iterator> smatch;
即smatch函数的本质是match_results<string::const_iterator>
smach用来保存匹配的结果

regex_match(string类，regex类)返回的是bool类型，如果成功匹配则返回True，若匹配失败则返回False
regex_search(string类，regex类)返回的是bool类型，如果成功匹配则返回True，若匹配失败则返回False
regex_match是全词匹配，而regex_search是搜索其中匹配的字符串
regex_replace是替换正则表达式匹配内容的函数，详细见文档和C++primer
*/
	int main(int argv,char *argc)
{
	std::smatch result;
	std::string str("Test.txt  Hello.txt  Goodbye.txt");
	std::regex reg("[A-Za-z]+\.txt");
	//std::cout<<std::regex_match(str,reg)<<std::endl;
	std::cout<<std::regex_search(str,result,reg)<<std::endl;
	for (std::sregex_iterator it(str.begin(),str.end(),reg),  end_it;    it!=end_it;    it++)
		//end_it自定义的变量名，是sregex_iterator类型,这里end_it为空,等效于迭代器的尾部(类似于end())
	    std::cout<< it->str() <<std::endl;
	system("pause");
	return 0;
}