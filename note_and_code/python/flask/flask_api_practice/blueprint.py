#-*-coding:utf-8-*-
"""
蓝本脚本必须和Flask的app所在的脚本分开，若写在同一个py文件
使用app.run()时,蓝本的视图函数不会被执行
"""
from flask import Blueprint

blue=Blueprint('main',__name__)

#@的是blue，不是main
@blue.app_errorhandler(404)
def page_not_found():
    return "<h1>You can't found the page</h1>",404

@blue.route('/blue2')
def blueview1():
    return "<h1>This blueprint views</h1>"
