void test(string& str);

传入参数时，不要传 test(string("hello"))；
而是:
str = "hello";
test(str);