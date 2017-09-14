from flask_script import Manager, Shell, Server
from flask_migrate import MigrateCommand
from app import create_app, db
from app.db import Author, Post, Image, Alembic

app = create_app("production")

manager = Manager(app)
manager.add_command("runserver", Server(use_debugger=True))


def make_shell_context():
    return dict(app=app, db=db, Author=Author, Post=Post, Image=Image, Alembic=Alembic)

manager.add_command("shell", Shell(make_context=make_shell_context))
manager.add_command("db", MigrateCommand)

if __name__ == "__main__":
    manager.run()