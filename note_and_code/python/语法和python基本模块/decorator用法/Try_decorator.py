#-*-coding:utf-8-*-
'''使用decorator'''

'''
def My_decorator(fun):
    def func1(*p1,**p2):
        print 'Let\'s see the rank'
        return fun(*p1,**p2)        #return拼错了会给fun报错，给人误导
    return func1


@ My_decorator
def now(*p1,**p2):
    print p1,p2

now('now',city='Beijing')
'''

def My_decorator(para):
    def func1(func):
        def func2(*p1,**p2):
            print 'get %s?' %para
            return func(*p1,**p2)
        return func2
    return func1

@ My_decorator('String')            #para参数是写在@后面的
def now(*p1,**p2):                  #相当于now=My_decorator('String')(now)
    print 'Let\s go'    
    print p1,p2

    '''
    执行的过程:now(''now',city='Beijing')
    找到My_decorator地址把('String')传入
    找到func1地址把定义的now函数传入
    找到func2地址，至此生成一个函数 now(*p1,**p2)
    return func(*p1,**p2)是返回其执行的结果，上述例子中返回空
    而其它函数return出其地址，是为了生成函数而用的
    '''
now('now',city='Beijing')           #插入的函数的参数是在要运行时写入的




