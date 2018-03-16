#coding:utf-8
import re

print re.findall\
(r'''(?i)href=["']?([^\s"'<>]+)''','<a href="www.loclahost.com">HAHA</a>')
# ‘\’要加上
result = re.match(r'''["'#$]''','#!')
if result is not None:
    print result.group()

#用两组三个单(或双)引号''' '''所表示的字符串中 !#全部会被当作字符
'''

>>>print """hello
world"""

hello
world
这样就不用写'hello\nworld'了



'''
