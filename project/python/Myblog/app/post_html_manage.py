import re
from base64 import b64decode
from .db import Author, Post, Image, db
from config import ProductionConfig as Config
from hashlib import md5
from flask import abort
from datetime import datetime
import os
from .logger import log


def compress_and_store_post(title, title_img_file, content, blog_type, profile, ctx):
    try:
        ctx.push()
        pattern = re.compile(r'(<img[^>]*?src=\")' + '[^>]*?;base64,' + '([^\"]*?)' + '(\"[^>]*?>)')
        blog = _compress_html(pattern, content)
        blog_id = _add_blog(title, title_img_file, blog["content"], blog_type, profile)
        _store_img_paths(blog_id, blog["img_paths"])
    except Exception as e:
        log('compress_and_store_post 发生未知错误')
        abort(404)
    finally:
        ctx.pop()


def compress_and_edit_post(title, title_img_file, content, blog_type, profile, post, ctx):
    try:
        ctx.push()
        pattern = re.compile(r'(<img[^>]*?src=\")' + '[^>]*?;base64,' + '([^\"]*?)' + '(\"[^>]*?>)')
        blog = _compress_html(pattern, content)
        _edit_blog(title, title_img_file, blog["content"], blog_type, profile, post)
        _edit_img_paths(post.id, blog["content"])
    except Exception as e:
        log('compress_and_edit_post 发生未知错误')
    finally:
        ctx.pop()


def remove_blog(post_id, ctx):
    try:
        ctx.push()
        post = Post.query.filter_by(id=post_id).first()
        images = Image.query.filter_by(post_id=post_id).all()
        if not post:
            abort(404)
        delete_blog_image("app"+post.title_img)
        for img in images:
            delete_blog_image("app"+img.image_path)
            db.session.delete(img)
        db.session.delete(post)
    except Exception:
        log("remove_blog 未知错误")
    finally:
        db.session.commit()
        ctx.pop()


def _add_blog(title, title_img_file, content, blog_type, profile):
    post = Post()
    post.author_id = Author.query.filter_by(username=Config.AUTHOR).first().id
    post.title = title
    post.content = filter_html(content)
    post.type = blog_type
    post.profile = profile
    title_img_path = store_blog_image(title_img_file, blog_type=blog_type)
    post.title_img = title_img_path
    db.session.add(post)
    db.session.commit()
    return post.id


def _edit_blog(title, title_img_file, content, blog_type, profile, post):
    post.title = title
    post.content = filter_html(content)
    post.blog_type = blog_type
    post.profile = profile
    post.latest_timestamp = str(datetime.now()).encode("utf-8")
    if title_img_file: # 如果用户提交了新的标题图片,则将原标题图片删除
        delete_blog_image("app" + post.title_img)
        title_img_path = store_blog_image(title_img_file, blog_type=blog_type)
        post.title_img = title_img_path
    db.session.add(post)
    db.session.commit()


def _compress_html(pattern, post_html):
    blog = {"content": "", "img_paths": []}
    img_tags = pattern.findall(post_html)
    if img_tags:
        for tag in img_tags:
            post_html = _exchange_b64img(pattern, post_html, tag, blog)
            # the base64 code is stored in tag[1]
    blog["content"] = post_html
    return blog


def filter_html(html):
    p1 = re.compile(r"<script>")
    p2 = re.compile(r"<script/>")
    filtered_html = p1.sub("&lt;script&gt;", html)
    filtered_html = p2.sub("&lt;script/&gt;", filtered_html)
    return filtered_html


def store_blog_image(img_file, blog_type=None):
    try:
        if img_file:
            img_name = _exchange_md5(img_file)
            img_path = "/static/img/" + img_name
            with open("app"+img_path, "wb") as f:
                f.write(img_file)
        # 如果作者没有为文章设计题目图片,则根据文章的类型选择默认的图片
        else:
            if blog_type == "life":
                img_path = '/static/img/system/life.jpg'
            elif blog_type == "programme":
                img_path = '/static/img/system/programme.jpg'
            else:
                img_path = '/static/img/system/science.jpg'
    except Exception:
        log("store_blog_image 图片写入文件夹失败")
        img_path = "/static/img/system/error.jpg"
    return img_path


def delete_blog_image(file_path):
    pattern = re.compile("system")
    result = pattern.search(file_path)
    if not result:
        try:
            os.remove(file_path)
        except FileNotFoundError:
            log("delete_blog_image 错误:未找到要删除的图片")
        except Exception:
            log("delete_blog_image 未知错误:删除图片失败")


def _store_img_paths(blog_id, img_paths):
    for img_path in img_paths:
        image = Image()
        image.post_id = blog_id
        image.image_path = img_path
        db.session.add(image)
    db.session.commit()


# 删除修改后文章中不需要的图片及其路径,已经将最新文章所有图片路径更新到数据库
def _edit_img_paths(blog_id, blog_html):
    dulplicated_paths = []
    pattern = re.compile("<img[^>]*?src=\"(/static/img/[0-9a-f]{32})[^>]*?>")
    new_img_paths = pattern.findall(blog_html)
    post = Post.query.filter_by(id=blog_id).first()
    images = Image.query.filter_by(post_id=blog_id).all()
    if post:
        for image in images:
            for new_img_path in new_img_paths:
                if image.image_path == new_img_path:
                    dulplicated_paths.append(new_img_path)
    # 将修改后文章中没有出现的图片删除
    for image in images:
        if image.image_path not in new_img_paths:
            delete_blog_image("app" + image.image_path)
            db.session.delete(image)
    # 删除新加入的路径列表中与原数据库重复的路径
    for dulplicated_path in dulplicated_paths:
        new_img_paths.remove(dulplicated_path)
    for new_img_path in new_img_paths:
        image = Image()
        image.image_path = new_img_path
        image.post_id = blog_id
        db.session.add(image)
    db.session.commit()


# 将图像的base64编码转化为md5值,将md5值作为文件名的一部分
def _exchange_md5(img_b64):
    time_stamp = str(datetime.now()).encode("utf-8")
    md = md5()
    if type(img_b64) is not bytes:
        md.update(time_stamp + img_b64.encode("utf-8"))  # python3字符串默认使用unicode,而unicode不能作为参数,要先转换
    else:
        md.update(time_stamp + img_b64)
    return md.hexdigest()


# 将<img>元素src内的b64编码转换为图像文件存到static文件夹中,并将src的内容由b64编码用图像的路径代替
def _exchange_b64img(pattern, post_html, img_tag, blog):
    try:
        img = b64decode(img_tag[1])
        img_path = store_blog_image(img)
        blog["img_paths"].append(img_path)
        post_html = pattern.sub(img_tag[0] + img_path + img_tag[2], post_html, 1)
        # 每次替换一个img元素,因为替换后img的src内为路径,没有base64,所以下一次这个src不会被替换
    except TypeError:
        print('转码错误!')
        log("_exchange_b64img base64转码失败,文件类型错误")
    except Exception:
        # 不能成功地提取img元素内的src转换为图片,因此直接将原html返回并存储
        log("_exchange_b64img base64转码失败,未知错误")
    finally:
        return post_html


