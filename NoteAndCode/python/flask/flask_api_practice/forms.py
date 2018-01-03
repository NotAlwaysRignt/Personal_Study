""" try to use @ property
class HelloWorld(object):

    def printSomething(self,para):
        print '%s' % para

    @property
    def attr(self):
        print('You can\' get it')

    @attr.setter
    def attr(self,value):
        self.anovalue=value
"""
from flask_bootstrap import Bootstrap 
from flask import Flask,render_template,session,redirect,url_for
from flask_wtf import Form
from wtforms import StringField,SubmitField,BooleanField
from wtforms.validators import Required

class NameForm(Form):
    remember_me=BooleanField('Keep me logged in')
    name=StringField("What's your Name?",validators=[Required()])
    submit=SubmitField('Submit')

app=Flask(__name__)
bootstrap=Bootstrap()
bootstrap.init_app(app)
app.config['SECRET_KEY']='hard to guess'

@app.route('/',methods=['GET','POST'])
def index():
    form=NameForm()
    if form.validate_on_submit():
        session['name']= form.name.data
        return redirect(url_for('index'))
    
    return render_template('index.html',form=form,name=session.get('name'))

if __name__=='__main__':
    app.run(debug=True)

        
