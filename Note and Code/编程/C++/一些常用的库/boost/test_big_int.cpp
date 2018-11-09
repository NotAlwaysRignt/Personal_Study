#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "gtest.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/lexical_cast.hpp"

using namespace std;
namespace mp = boost::multiprecision;

TEST(boost,cpp_int)
{
    mp::cpp_int Big =1;
    for (size_t i = 0; i < 100; ++i) {
        Big*=100;
    }

    string strBig = boost::lexical_cast<string> (Big);
    std::cout << strBig << std::endl;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
