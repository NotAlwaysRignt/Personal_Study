#coding:utf-8
#python中,(不需要导入)全局可用的函数getattr(a,"b")等价与a.b
#注意第二个参数要写成字符串形式

#而在类中,__getattr__方法,会在你访问类的成员变量时被触发,就好像运算符的重写
#让你在访问类成员时有不一样的行为

#setattr(obj,"a",b)等价与 obj.a = b 注意第二个参数要写成字符串形式
#在类中重写了__setattr__方法,会在你改变类的成员变量时触发这个函数

#hasattr(obj,"a") 判断obj中是否有成员a(第二个参数a要写成字符串),如果有则
#返回True,否则返回False


#hasattr,getattr和setattr都是 成员变量名参数 要写成字符串形式
class A(object):
    def __init__(self):
        self.value = "this is a"

a = A()
print(getattr(a,"value"))
setattr(a,"value","it is now change")
print(a.value)
if hasattr(a,"value"):
    print("yes,there is value");
if not hasattr(a,"val"):
    print("there is no val");
#输出结果
#this is a
#it is now change
#yes,there is value
#there is no val

