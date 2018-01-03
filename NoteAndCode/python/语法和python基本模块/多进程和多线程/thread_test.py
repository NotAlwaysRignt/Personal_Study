#-*-coding:utf-8-*-
import threading

#方式一，创建threading.Thread类的实例

def myfunc(arg='??'):
    print 'the first way:'+' the argument:%s' %arg

t1 = threading.Thread(target = myfunc,name='mythread',args=('m_arg',))
#target是要执行的线程，name是线程的名字，可以自己取，args是要传入的参数
#args要传入一个tuple，注意这里只有一个参数的时候写成('m_arg',) ,要加逗号
#否则会被认为是一个字符串加了括号


#方式二，创建一个自定义的类继承threading.Thread:
class test_thread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True
        #daemon,守护进程,设为True后,如果主线程退出，守护线程也会自动退出
        self.start()

    def run(self):#命名必须为run，才会被执行
        print 'the second way'

if __name__=='__main__':
    t1.start()
    t2=test_thread()
