#coding:utf-8
'''
多继承中,当调用self.方法的时候,总是会先从最顶级的子类中搜寻方法,如果在子类中找到了这个方法
那么就调用它,并且其父类的同名的方法都不再被执行
'''

class A(object):
    def Print(self):
        self.func()
        
class B(A):
    def func(self):
        print("B")

class C(B):
    def func(self):
        print("C")
        
c = C() 
c.Print()

'''
运行结果:
C
'''

