#coding:utf-8
#简单的说,__slots__用于限制一个类的变量的名字

class A():
    __slots__=('a','b')
    
a = A()
a.a="a"
print (a.a)

a.b = "b"
print (a.b)

a.c ="c"
'''
结果:
a
b
Traceback (most recent call last):
  File "__slot__.py", line 14, in <module>
    a.c ="c"
AttributeError: 'A' object has no attribute 'c'

'''
