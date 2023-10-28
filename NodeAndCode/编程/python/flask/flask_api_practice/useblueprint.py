#-*-coding:utf-8-*-
from flask import Flask
from blueprint import blue 

app=Flask(__name__)


@app.route('/1')
def view1():
    return "<h1>Here is app, view1</h1>"

app.register_blueprint(blue)


if __name__=='__main__':
    app.run()

