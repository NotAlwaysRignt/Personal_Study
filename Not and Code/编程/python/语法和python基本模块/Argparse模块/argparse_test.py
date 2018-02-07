#coding:utf-8
import argparse

parser = argparse.ArgumentParser()
#定位参数
parser.add_argument("echo",help = "echo the string you enter")
parser.add_argument("my_arg",type=int)
#可选参数，在参数名前加“--”，则该参数为可选参数
#如"--changable"，在指定--chanable时要在后面加上一个字符作为参数
parser.add_argument("--changable")
parser.add_argument("-v","--verbose")#-v是--verbose的简写
parser.add_argument("--ver",action = "store_true")

args = parser.parse_args()
print "echo: " + args.echo
print "my_arg: "
print args.my_arg

if args.changable:# 命令行中--changable 10，则args.chanable为"10",是字符串
    print "changable: " + args.changable

if args.verbose:
    print "verbose:" + args.verbose

print args.ver
#--ver指定action = "store_true",则--ver在后面不可以为其添加参数
#如果在命令行中有--ver，则args.ver为True，若输入命令行参数中无--ver，
#则args.ver值为False

'''
argparse.ArgumentParser(),返回一个解析器对象

add_argument("echo",help = "echo the string you enter")
可添加一个echo命令，help的内容是在使用-h命令时会输出的帮助信息

parser.add_argument("echo",help = "echo the string you enter")
parser.add_argument("my_arg")
当执行python argparse_test.py hello world
则会根据先后执行add_argument的顺序认定hello为echo，world为my_arg
默认情况下echo，my_arg都是字符串

若要读入int型参数，则可以用
parser.add_argument("my_arg",type=int)

定位参数和可选参数间不需要顺序(定位参数间要遵照add_argument()执行的顺序)
可选参数
'''
