from flask import Flask
from flask_bootstrap import Bootstrap
from flask_migrate import Migrate
from flask_login import LoginManager
from flask_wtf.csrf import CSRFProtect
from config import config
from .db import db

bootstrap = Bootstrap()
csrf = CSRFProtect()
login_manager = LoginManager()

def create_app(config_name):
    app = Flask(__name__)
    app.config.from_object(config[config_name])
    config[config_name].init_app(app)

    # plugins initialize
    db.init_app(app)
    Migrate(app, db)

    from .main import main
    app.register_blueprint(main)

    from .blog import blog
    app.register_blueprint(blog, url_prefix="/blog")
    
    from .searchsite import searchsite
    app.register_blueprint(searchsite,url_prefix="/searchsite")
    
    bootstrap.init_app(app)
    csrf.init_app(app)
    login_manager.init_app(app)

    return app
