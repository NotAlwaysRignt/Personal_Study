#-*-coding:utf-8-*-
from flask import Flask,render_template
from flask_mail import Mail,Message
'''
这里不是smtp.exmail.qq.com
SSL或其它安全协议至少得开启一个，不然会报错
msg.body发送的是文本 <h1>等html不会被翻译而是当作邮件内容写入
msg.html写入的是html文本，<h1>等tag会被翻译成网页格式，以网页作为邮件内容
如果用模板，body发txt文件，html发html文件
当body和html都有内容时，发出的邮件内容默认为html的内容
Message()的第一个参数是发送邮件的标题
'''
app=Flask(__name__)     
app.config.update(\
    MAIL_SERVER='smtp.qq.com',\
    MAIL_PORT=465,\
    MAIL_USE_SSL= True,
    MAIL_USERNAME='chaoran724949515@qq.com',\
    MAIL_PASSWORD='bdypdxioblwpbbeg')#IMAP :alowikrvjoasbeji
                                        #POP3:bdypdxioblwpbbeg
mail=Mail(app)

@app.route('/')
def send_mail():
    msg=Message('这是标题',sender=app.config['MAIL_USERNAME'],\
                 recipients=['724949515@qq.com'])
    msg.body=render_template('mail_content.txt')

    mail.send(msg)
    return "<h1>Have you gotten the mail?</h1>"

if __name__=='__main__':
    app.run(debug=True)
