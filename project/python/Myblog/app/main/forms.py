from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, SubmitField, BooleanField
from wtforms.validators import DataRequired, Length


class LoginForm(FlaskForm):
    username = StringField("用户名", validators=[DataRequired(), Length(1, 32)])
    password = PasswordField("密码", validators=[DataRequired()] )
    remember_me = BooleanField('记住我')
    submit = SubmitField(u'登录')