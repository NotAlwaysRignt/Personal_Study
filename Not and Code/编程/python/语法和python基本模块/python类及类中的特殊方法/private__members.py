#coding:utf-8
#python类的成员变量中
#如果是双下划线开头的,则为私有成员变量

class A(object):
    def __init__(self):
        self.__a = "123"
        object.__setattr__(self,"_A__b","456")

    def Prin(self):
        print("a is "+self.__a);
        print("b is "+self.__b);

#如上面的例子,

a = A()
#a.__a 是无法访问成功的,其实当你在内部使用self.__XXX时
#python是将这个变量名改成了_类名__XXX (第一处只有一跟下划线)

print (a._A__a)

#另一方面,如果在函数内部使用object.__setattr__(self,"_A__b","456")
#其实是相当于self.__b = "456"
print (a._A__b)
a.Prin()

