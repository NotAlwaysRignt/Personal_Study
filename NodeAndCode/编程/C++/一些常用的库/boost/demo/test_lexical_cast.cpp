#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "gtest.h"
#include <boost/lexical_cast.hpp>

using namespace std;

TEST(test,exception)
{
    int i;
    try {
        i = boost::lexical_cast<int>("123.456"); //double字符串是没办法强制转换成int 的
    }catch(boost::bad_lexical_cast& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << i << std::endl;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

