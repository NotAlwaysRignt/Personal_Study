#coding:utf-8
head ="""GET / HTTP/1.1
Accept: */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Host: www.google.cn
Connection: Keep-Alive"""


headers = dict(h.split(': ') for h in head.decode().split('\n')[1:])


str2="hello world and good morning"

'''
这里有一个很奇特的用法
l=['a b'] #a b间有个空格
for h in l:
    h.spilt(' ') #' ' 内有一个空格
只有一个h，split()返回一个列表，并将字符串根据参数进行划分后放进列表里，
此处打印['a','b']
结合dic()方法可以这样做d = dict(h.split(' ') for h in l)  注意' '内有个空格
则d = {'a':'b'}
如果l为['a b','c d'],则仿照上述步骤可得d={'a':'b','c':'d'}
>>>head.decode()
GET / HTTP/1.1
Accept: */*
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Host: www.google.cn
Connection: Keep-Alive

>>>head.decode().split('\n')
[u'GET / HTTP/1.1', u'Accept: */*', u'Accept-Language: zh-cn',
u'Accept-Encoding: gzip, deflate', u'User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)',
u'Host: www.google.cn', u'Connection: Keep-Alive']
一个列表

>>>head.decode().split('\n')[1:]
[u'Accept: */*', u'Accept-Language: zh-cn', u'Accept-Encoding: gzip, deflate',
u'User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)',
u'Host: www.google.cn', u'Connection: Keep-Alive']
一个列表


'''
