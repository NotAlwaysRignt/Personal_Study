from datetime import datetime
from flask import current_app


def log(message):
    time = str(datetime.now())
    time = time+": "
    message = time + message
    current_app.logger.warning(message)