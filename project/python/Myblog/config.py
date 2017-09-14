import logging

class Config:
    # remember to delete secret key when upload to github.
    SQLALCHEMY_DATABASE_URI = "mysql+pymysql://MrRan:123@localhost/MyBlog"
    SECRET_KEY = "your secret key"
    WTF_CSRF_SECRET_KEY = "your csrf_key"

    SQLALCHEMY_TRACK_MODIFICATIONS = True

    @staticmethod
    def init_app():
        pass


class ProductionConfig(Config):
    AUTHOR = "Mr Ran"
    POST_PER_PAGE = 5
    MAX_CONTENT_LENGTH = 32 * 1024 * 1024 # max upload file = 32M

    @staticmethod
    def init_app(app):
        # 创建日志文件, 用于记录日志
        handler = logging.FileHandler("warning.log")
        app.logger.addHandler(handler)



config = {"production": ProductionConfig}
