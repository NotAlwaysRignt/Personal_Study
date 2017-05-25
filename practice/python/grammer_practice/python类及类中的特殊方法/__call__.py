#coding:utf-8
#python 中,函数其实是一个可以调用的对象
def func():
    pass

f=func    
print(f.__name__)
#结果:func

#__call__ 方法使得类的实例也成为可调用的对象,通俗的说__call__使类的对象可以像函数一样被调用

class A():
    def __call__(self,a):
        print("you use the class as a function")
        print("the argument you give is "+a)
        
a=A()
a("hello")
#结果:
#you use the class as a function
#the argument you give is hello
