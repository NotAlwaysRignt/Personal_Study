#-*-coding:utf-8-*-
from flask import Flask
import os
basedir=os.path.abspath(os.path.dirname(__file__))

#print os.path.dirname(__file__)
#print basedir

class Config:
    SECRET_KEY=os.environ.get('SECRET_KEY') #or 'hard to guess'
    CONFIG_NAME_ONE='myconfig1'
    CONFIG_NAME_TWO='myconfig2'
    CONFIG_NAME_THREE='myconfig3'

    @staticmethod
    def init_app(app):
        pass

app=Flask(__name__)

#app.config.from_object('config.Config')

#print app.config['CONFIG_NAME_ONE']
'''
app.config.from_object('config.Config')
写明脚本和脚本里的类，从类中获取信息,变量名可以有数字，但不可以有小写字母
print app.config['CONFIG_NAME_ONE']
这里可以打印出myconfig1

'''

'''
from_envvar('vary')中的参数vary要先在命令行中用‘set vary=文件绝对路径’ 设置好
从cfg文件中导入，cfg文件可用记事本写好后用另存为创建，内容直接写配置极其值
例：
CONFIG_NAME_ONE='myconfig1'
CONFIG_NAME_TWO='myconfig2'
不同变量用回车隔开，注意变量前面不能有空格，否则会报错
app.config.from_envvar('myvary')
print app.config['CONFIG_NAME_ONE']
这里可以打印出myconfig1
'''


        
