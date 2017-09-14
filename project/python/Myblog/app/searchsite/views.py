from . import searchsite
from .forms import Form
from flask import render_template, request, current_app, url_for, redirect, jsonify
from app import login_manager
from flask_login import login_user, logout_user, login_required, current_user
from .spider.spider import spider
from .spider.config import city

@searchsite.route('/',methods=['GET','POST'])
def deault_index(): #默认跳转到广州的搜索界面
    cityid="4"
    return redirect(url_for('.index', cityid='4', city=city[cityid]))


@searchsite.route('/search/<cityid>',methods=['GET'])
def index(cityid):
    return render_template('searchsite/index.html',cityid=cityid,city=city[cityid])
    
@searchsite.route('/<cityid>',methods=['GET'])
def search(cityid):
    site = request.args.get("site",None) #表单用的是GET方法而不是POST方法，所以不用request.form
    if site:
        url = "http://www.dianping.com/search/keyword/"+\
                        cityid+"/0_"+site
        shop_list = spider.get_shop_info(url)
    else:
        shop_list=[]
        url=''
    return render_template("searchsite/shop.html",
                    shops={"shop_info":shop_list,"official_url":url},
                    cityid=cityid,city=city[cityid])


@searchsite.route('/map')
def map():
    cityid = request.args.get("cityid","4")
    address = request.args.get("address","")
    return render_template("searchsite/map.html", city=city[cityid],destination=city[cityid]+address)


@searchsite.route('/comment')
def comment():
    url = request.args.get("comment_url")
    comments = spider.get_user_comment(url)
    return render_template("searchsite/_comment.html", comments=comments)
