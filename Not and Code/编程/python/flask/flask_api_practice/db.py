#-*- coding:utf-8 -*-
from flask_sqlalchemy import SQLAlchemy
import os
from flask import Flask
from flask_script import Shell,Manager

basedir=os.path.abspath(os.path.dirname(__file__))
ubasedir= basedir.decode('gbk')
#sqlite3不支持中文路径，若路径有中文，要转换成unicode编码
app=Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI']=\
        'sqlite:///'+os.path.join(ubasedir,'data.sqlite')
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN']=True
app.config['SQLALCHEMY_TRACK_MODIFICATIONS']=True
db=SQLAlchemy(app)

class User(db.Model):
    id=db.Column(db.Integer,primary_key=True)
    username=db.Column(db.String(80),unique=True)

    def __init__(self,id,username):
        self.id=id
        self.username=username

manager=Manager(app)
def make_shell_context():
    return dict(app=app,db=db,User=User)
manager.add_command("shell",Shell(make_context=make_shell_context))

if __name__=='__main__':
    manager.run()







