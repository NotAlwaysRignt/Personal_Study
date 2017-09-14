from flask import Blueprint

searchsite = Blueprint('searchsite', __name__)
from . import views
