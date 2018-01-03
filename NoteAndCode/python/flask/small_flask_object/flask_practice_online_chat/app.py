
from flask_bootstrap import Bootstrap
from flask import Flask, request, session, \
    render_template, Response, jsonify, redirect, flash
import time
from Allmessages import AllMessages

all_messages = AllMessages()
app = Flask(__name__)
Bootstrap(app)
DELAY_TIME = 5


@app.route('/')
def index():
    return render_template('index.html')

@app.route("/update",methods = ["GET", "POST"])
def update():
    checktime = 1
    s_timestamp = request.args.get("timestamp", -1)
    try:
        timestamp = float(s_timestamp)
    except:
        return jsonify(message=[], latest_time=all_messages.lateset_time())

    while(checktime < DELAY_TIME):
        data = all_messages.check_time(timestamp)
        if data:
            return jsonify(data)
        time.sleep(1)
        checktime = checktime+1
    return jsonify(message=[], latest_time=timestamp)


@app.route("/add", methods=["POST"])
def add():
    content  = request.form.get("message","error")
    name = request.form.get("name","Anonymous")
    all_messages.add(content, name)
    #return ...

@app.route("/test")
def test():
    data = request.args.get("timestamp", -1)

    print(data)
    return jsonify(dict(timestamp = data))

if __name__ == '__main__':
    app.run()
