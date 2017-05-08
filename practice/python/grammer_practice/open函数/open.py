#-*-coding:utf-8-*-
f=open('line.txt')
data = f.readline()
print data
print len(data)

'''
f=open('test.txt')

print f.next()
print f.next()
print f.next()
'''

'''
for line in f:
    print line
'''

'''

for another_line in f:
    if another_line.startswith('mark'):
        print another_line 
'''

'''
用for line in f迭代完毕后 for another_line in f就不能再进行迭代
因为用for line in f迭代后，文件的指针都会向后移动，当用for迭代完毕后，文件的
指针已经移动到了文件的末尾，可以在用for line in f后再用f.next()检验，会报错
'''
