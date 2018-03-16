#coding:utf-8
#偏函数,其实就是把一个函数的某些参数给绑定了一些默认值
#然后生成一个新的函数
#偏函数在functools模块中
import functools
def func(a,b):
    print("a is "+a)
    print("b is "+b)

fun2 = functools.partial(func,a="A")

fun2(b="B")

#结果
#a is A
#b is B

