#include"gtest/gtest.h"
#include"Add.cpp"
TEST(funtion,add)
{
    Add a;
    EXPECT_EQ(1,a.add_func(0,1));
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
