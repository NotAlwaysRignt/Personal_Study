#coding:utf-8

b = 'b'
def fun():
  while (True):
    print b
    a = yield b
    print a
    print b


f = fun()
#f.send(None)
#f.send("Hello")

"""
第一次启动协程只能用send(None)

这样理解yield: 运行到yield时，相当于把yield后的变量return出去
此处每运行到a = yield b时，就把b return了出去作为返回值赋给调用send的生成器

当使用send再次启动协程时，send()里面的参数将作为yield的值，

假设第一次 a = yield b处停止将b 返回出去，那么当调用f.send("Hello")时
可以看成 a = "Hello",注意send传进来的参数相当于把yield整一块替换掉了，并不会把值
赋给yield后面的变量，即此处b始终都是'b'，调用了f.send("hello") b的值不会被
附成"hello"

"""
