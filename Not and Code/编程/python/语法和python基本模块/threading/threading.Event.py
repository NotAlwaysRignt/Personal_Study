#coding:utf-8
'''
python的threading.Event()用于管理线程之间事件发生的顺序
对于一个Event的对象event,当一个线程中使用event.wait()时,它会停止在那
直到event.set()被执行

注意,当运行了一次event.set()后,后面所有的event.wait()都不会阻塞
除非运行了event.clear(),则后面的event.wait()又会阻塞直到event.set()的运行

这里我们可以大致猜测这种阻塞机制,当运行到event.wait()时,会去检查一个bool变量
假设默认为False,即False时阻塞,wait()会不断询问这个bool变量知道它会True才往下运行
set()可以将其置为True,而要重置为False,则要用clear()

有了这个方法,我们在多线程的编程中,如果需要某个线程A先完成准备工作,其它线程才能开始工作
则可以让其它线程先wait(),知道线程A完成准备工作并使用了set(),其它线程才会开始
'''

import threading    
import time    
    
def worker(event,threadname):                                                   
    print(threadname + ' wait for the set')                                     
    event.wait()                                                                
    print (threadname+' you used the set')                                      
    time.sleep(2)                                                               
    print (threadname + " finished")                                            
                                                                                
event = threading.Event()                                                       
t1 = threading.Thread(target = worker,args=(event,'thread1'),name = "t1")       
t1.start()                                                                      
t2 = threading.Thread(target = worker,args=(event,'thread2'),name = "t2")       
t2.start()                                                                      
                                                                                
print("before using set()")                                                     
event.set()                                                                     
print("after using set()")


'''
运行结果:

thread1 wait for the set
thread2 wait for the set
before using set()
after using set()
thread1 you used the set
thread2 you used the set
thread1 finished
thread2 finished

'''
#再看一个用法
"""
import threading
    
event = threading.Event()    
print("use set")    
event.set()    
    
event.wait()    #不会阻塞
    
print("would it be printed?")#会被打印    
event.clear()    
event.wait()    #会阻塞
print("would it be used again?")#不会被打印 
"""
