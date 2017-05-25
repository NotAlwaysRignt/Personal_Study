#coding:utf-8
#类中存在默认的__getattr__ 和__setattr__方法,
#__getattr__ 在访问类中不存在的成员时被调用(注意是不存在),或者在类中主动去调用它
#__setattr__在对类的成员进行赋值时被调用
#我们通过自定义__getattr__ 和__setattr__方法,就改变了类的成员被访问和赋值时的行为,
#就好像重写了操作符一样

class A(object):
    def __init__(self):
        self.a = 123
        
    def tryget(self):
        self.__getattr__("")
        
    def __getattr__(self,name):
        print("you try to visit the member")
        if name=="b":
            print ("you try to visit b,but there isn't")
        #切记,__getattr__里面如果有self.XX的语句,则相当于访问自己的成员,如果这个成员
        #是不存在的,则又会在这时调用__getattr__,可能会导致无限的迭代直到栈溢出

a = A()
a.b
a.tryget()

'''
结果
you try to visit the member
you try to visit b,but there isn't
you try to visit the member
'''

class B(object):
    def __init__(self):
        pass
        
    def __setattr__(self,name,value):
        print("you try to change memeber")
        
b = B()
b.val = 456
print (b.val)
'''
结果如下,因为在__setattr__中只是有打印操作,而并没有定义赋值操作,所以b.val=456这一步并没有
把val绑定当b对象中
you try to change memeber
Traceback (most recent call last):
  File "about__attr__.py", line 42, in <module>
    print (b.val)
AttributeError: 'B' object has no attribute 'val'

如果把__setattr__注释掉,则结果:
you try to visit the member
456

要小心别在__setattr__中使用self.XX=..,否则又会进入__setattr__,否则可能会不断迭代导致栈溢出
'''

