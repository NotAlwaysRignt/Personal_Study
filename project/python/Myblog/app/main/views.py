from . import main
from flask import render_template, request, current_app, url_for, redirect
from ..db import Post, Author
from app import login_manager
from .forms import LoginForm
from flask_login import login_user, logout_user, login_required, current_user


@main.route("/")
def index():
    page = request.args.get('page', 1, type=int)
    blog_type = request.args.get("blogtype")
    # 按时间降序显示全部文章
    if blog_type:
        pagination = Post.query.filter_by(\
                author_id=Author.query.filter_by(username="Mr Ran").first().id, type=blog_type)\
                .order_by(Post.latest_timestamp.desc())\
                .paginate(page, per_page=current_app.config['POST_PER_PAGE'], error_out=False)
    # 按选择的类别并按时间降序显示全部文章
    else:
        pagination = Post.query.filter_by(\
                author_id=Author.query.filter_by(username="Mr Ran").first().id)\
                .order_by(Post.latest_timestamp.desc())\
                .paginate(page, per_page=current_app.config['POST_PER_PAGE'], error_out=False)
    posts = pagination.items
    return render_template("main/index.html", pagination=pagination, posts=posts)
# 如何没有找到任何结果,paginate函数仍会返回一个Pagination对象,只不过这个对象的items属性为空的[]
# pagination.items里存的是才是post对象数组,而pagination.iter_page()是页数


@login_manager.user_loader
def load_user(user_id):
    author = Author.query.filter_by(id=user_id).first()
    return author


@main.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm()
    if form.validate_on_submit():
        user = Author.query.filter_by(username=form.username.data).first()
        if user is not None and user.verify_password(form.password.data):
            login_user(user, form.remember_me.data)
            # 把user传入login_user，这样便会给该用户创建一个current_user
            return redirect(url_for('main.index'))
    return render_template('main/login.html', form=form)


@main.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('main.index'))
    


