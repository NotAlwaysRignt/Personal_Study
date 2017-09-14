from flask_wtf import FlaskForm
from wtforms import StringField, FileField, SubmitField, SelectField
from wtforms.validators import DataRequired, Length


class PostForm(FlaskForm):
    title = StringField("文章标题", validators=[DataRequired(), Length(1, 32)])
    post = StringField("你的文章", validators=[DataRequired()])
    profile = StringField("文章简介", validators=[Length(0, 128)])
    title_img = FileField()
    blog_type = SelectField("文章类型", choices=[("life", "生活"), ("programme", "编程"),
                                             ("science", "趣味科普")])