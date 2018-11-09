#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "gtest.h"
#include <boost/algorithm/string.hpp>

using namespace std;

TEST(test,upper)
{
    string str = "hello world";
    boost::to_upper(str);
    std::cout << str << std::endl; //结果存在原来的 string 中

    string str1 = "another str";
    const string& str2 = boost::to_upper_copy(str1); //必须是const 引用,结果存放在返回值中
    std::cout << str1 << std::endl; //不变
    std::cout << str2 << std::endl; //大写的 str1
}

TEST(test,starts_with)
{
    string str("hello world");
    bool start_with = false;
    start_with = boost::starts_with(str,"hello");
    if(start_with)
        std::cout << "str1 Start with hello" << std::endl;
    start_with = false;
    start_with = boost::istarts_with(str,"HELLO");
    if (start_with) {
        std::cout << "str1 Start with HELLO" << std::endl;
    }

    string str2 = "   hello world"; //前面有空格
    start_with = boost::starts_with(str2,"hello"); //start_with == false
    if(start_with)
        std::cout << "str2 Start with hello" << std::endl;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
