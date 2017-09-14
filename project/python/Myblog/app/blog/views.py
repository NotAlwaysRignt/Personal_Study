from . import blog
from flask import render_template, redirect, url_for, request, abort
from flask_login import login_required
from ..db import db, Author, Post
from ..blog.forms import PostForm
from threading import Thread
from ..post_html_manage import compress_and_store_post, compress_and_edit_post, remove_blog
#from manage import app


@blog.route("/writeblog", methods=["GET", "POST"])
@login_required
def write_blog():
    form = PostForm()
    if form.validate_on_submit():
        from manage import app
        ctx = app.app_context()
        t = Thread(target=compress_and_store_post,
                   args=[form.title.data, form.title_img.data.read(), form.post.data, form.blog_type.data,
                         form.profile.data, ctx])
        t.start()
        return redirect(url_for("main.index"))
    return render_template("blog/write_blog.html", form=form)


@blog.route("/showblog")
def show_blog():
    id= request.args.get("id")
    post = Post.query.filter_by(id=id).first()
    if not post:
        abort(404)
    return render_template("blog/blog.html", post=post)


@blog.route("/deleteblog")
@login_required
def delete_blog():
    id = request.args.get("id")
    if not id:
        abort(404)
    from manage import app
    ctx = app.app_context()
    t = Thread(target=remove_blog, args=[id, ctx])
    t.start()
    return redirect(url_for('main.index'))


@blog.route("/editeblog", methods=["GET", "POST"])
@login_required
def edit_blog():
    form = PostForm()
    post_id = request.args.get("id")
    post = Post.query.filter_by(id=post_id).first()
    if not post:
        abort(404)
    if form.validate_on_submit():
        from manage import app
        ctx = app.app_context()
        compress_and_edit_post(form.title.data, form.title_img.data.read(), form.post.data, form.blog_type.data,
                               form.profile.data, post, ctx)
        return redirect(url_for("blog.show_blog", id=post_id))
    return render_template("blog/write_blog.html", form=form, post=post)