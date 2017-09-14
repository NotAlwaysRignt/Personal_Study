from flask_sqlalchemy import SQLAlchemy
from flask_login import UserMixin
from datetime import datetime
from werkzeug.security import generate_password_hash, check_password_hash

db = SQLAlchemy()


class Alembic(db.Model):
    __tablename__ = 'alembic_version'
    version_num = db.Column(db.String(32), primary_key=True, nullable=False)


    @staticmethod
    def clear_db_versions_record():
        for db_version in Alembic.query.all():
            print (db_version.version_num)
            db.session.delete(db_version)
        db.session.commit()


class Post(db.Model):
    __tablename__ = 'posts'
    id = db.Column(db.Integer, primary_key=True)
    origin_timestamp = db.Column(db.DateTime(), default=datetime.now)
    latest_timestamp = db.Column(db.DateTime(), index=True, default=datetime.now)
    title = db.Column(db.Unicode(32))
    type = db.Column(db.String(32))
    title_img = db.Column(db.String(64))
    content = db.Column(db.Text)
    profile = db.Column(db.String(128))

    author_id = db.Column(db.Integer, db.ForeignKey('author.id'))
    images = db.relationship("Image", backref="posts", lazy="dynamic")

    def __repr__(self):
        return "<Post %r>" % self.title


# 这个表存储文章中包含图片的路径,以便在删除文章时将图片删除
class Image(db.Model):
    __tablename__ = "images"
    id = db.Column(db.Integer, primary_key=True)
    image_path = db.Column(db.String(128))
    post_id = db.Column(db.Integer, db.ForeignKey("posts.id"))

    def __repr__(self):
        return "<Image %r>" % self.image_path


class Author(db.Model, UserMixin):
    __tablename__ = 'author'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.Unicode(32), unique=True)
    password_hash = db.Column(db.String(256))
    about_me = db.Column(db.Unicode(300))
    posts = db.relationship('Post', backref="author", lazy='dynamic')

    def __init__(self, username, password):
        self.username = username
        self.password = password

    @property
    def password(self):
        raise AttributeError("Password is not accessed!")

    @password.setter
    def password(self, password):
        if len(password) > 30:
            raise ValueError
        self.password_hash = generate_password_hash(password)

    def verify_password(self, password):
        return check_password_hash(self.password_hash, password)

    def __repr__(self):
        return "<Author %r>" % self.username


# 暂未使用
class Series(db.Model):
    __tablename__ = 'series'

    id = db.Column(db.Integer, primary_key=True)
    series_id = db.Column(db.Integer)
    current_post_id = db.Column(db.Integer)
    next_post_id = db.Column(db.Integer)
    pre_post_id = db.Column(db.Integer)
    title = db.Column(db.String(32))

    def __repr__(self):
        return "<Series %r>" % self.title