import requests
from bs4 import BeautifulSoup
import re

from time import sleep
from browser import Browser
from parser import Parser
from config import pattern, city_id, DZ_headers


class Spider:
    
    def __init__(self):
        self.parser = Parser(pattern)
    
    # 参数，商品网页的url
    def get_shop_info(self, url):
        shop_info = []
        b = Browser(url)
        ul_tag = self.parser.search(b.get_text() ,"shops_ul")
        if ul_tag:
            list_tags = self.parser.find_all(ul_tag, "list")
            
            info = ["shop_name", "shop_url", "shop_type", 
                        "address", "money", "star", "img"] #需要查找的内容
            for li in list_tags:              
                temp = self.parser.search_from_list(li, info)
                shop_info.append(temp)
        return shop_info
        
    
    #参数： 用户评论所在网页的url
    #返回一个列表，列表里每一个元素都是一个字典，字典里存储着
    def get_user_comment(self, url):
        user_comments = []
        b = Browser(url,headers=DZ_headers)
        li_tag = self.parser.find_all(b.get_text(),"comment_li")
        if li_tag:  
            info = ["username","user_star","comment"]
            for li in li_tag:
                temp = self.parser.search_from_list(li,info)
                user_comments.append(temp)
        return user_comments
        
    
    def get_city_id(self, url="http://www.dianping.com/"):
        headers={'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0',
                'Connection':'keep-alive','Accept-Language':'en-US,en;q=0.5',
                'Accept-Encoding':'gzip, deflate',
                'Cookie':'cy=4; cye=guangzhou;'}
        # 不同于default_headers,这里我们要加一个cookie，cy=4（广州的id），cye=guangzhou，这样
        #大众点评网就知道我们地区位于中国，选择列表是中国城市而不是世界城市
        city = {}
        b = Browser(url,headers)
        soup = BeautifulSoup(b.get_text(),"html.parser")
        result = soup.find_all('a',onclick=re.compile("pageTracker._trackPageview\('dp_head_hotcity"))
        for r in result:
            url = r.get("href")
            b = Browser("http:"+url,headers)
            city_id = self.parser.search(b.get_text(),"city_id")
            if city_id:
                city[r.string]=city_id
                print(r.string+":"+city_id)
            sleep(1)
        print(str(city))
        return str(city)         

spider = Spider()
