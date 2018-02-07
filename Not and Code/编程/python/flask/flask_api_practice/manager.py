from flask_script import Manager,Command
from flask import Flask

app=Flask(__name__)

manager = Manager(app)

class Hello(Command):

    def run(self):
        print "Let's see if all the function will be conducted"


manager.add_command('hello',Hello())

if __name__=='__main__':
    manager.run()

