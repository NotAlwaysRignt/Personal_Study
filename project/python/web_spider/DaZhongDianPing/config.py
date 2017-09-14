import re

city = {"1":"上海市", "2":"北京市", "4":"广州市", "7":"深圳市"}

city_id = {'北京': '2', '天津': '10', '沈阳': '18', '大连': '19', '长春': '70', '哈尔滨': '79', '石家庄': '24', '太原': '35', '呼和浩特': '46', '廊坊': '33', '上海': '1', '杭州': '3', '南京': '5', '苏州': '6', '无锡': '13', '济南': '22', '厦门': '15', '宁波': '11', '福州': '14', '青岛': '21', '合肥': '110', '常州': '93', '扬州': '1', '温州': '101', '绍兴': '104', '嘉兴': '102', '烟台': '1', '威海': '152', '镇江': '98', '南通': '94', '金华': '105', '徐州': '92', '潍坊': '1', '淄博': '145', '临沂': '155', '马鞍山': '114', '台州': '108', '泰州': '99', '济宁': '1', '泰安': '151', '成都': '8', '武汉': '16', '郑州': '160', '长沙': '344', '南昌': '1', '贵阳': '258', '西宁': '313', '重庆': '9', '西安': '17', '昆明': '267', '兰州': '1', '乌鲁木齐': '325', '银川': '321', '广州': '4', '深圳': '7', '佛山': '208', '珠海': '1', '东莞': '219', '三亚': '1', '海口': '1', '南宁': '224', '惠州': '213', '香港': '341', '澳门': '342', '台北': '2335', '高雄': '2337', '垦丁': '2406', '花莲': '2336', '东京': '2372', '首尔': '2369', '曼谷': '2342', '新加坡': '2311', '吉隆坡': '2359', '墨尔本': '2322', '京都': '2373', '大阪': '2374', '北海道': '2375', '清迈': '2345', '芭堤雅': '2344', '普吉岛': '2343', '济州岛': '2371', '长滩岛': '2355', '巴厘岛': '2351', '沙巴': '2362', '纽约市': '2395', '洛杉矶': '2397', '巴黎': '2388', '伦敦': '2464', '悉尼': '2379', '奥克兰': '2384'}


pattern = {
        "city_id":re.compile("data-s-cityid=\"([0-9]*)\""),
        "shops_ul": re.compile("<div class=\"shop-list J_shop-list shop-all-list\" id=\"shop-all-list\">.*?<ul>(.*?)</ul>.*?</div>",re.S),    # 商店列表
        "comment_li": re.compile("<li id=\"rev_\d*\" data-id=\"\d*\" ?>(.*?)</li>",re.S), # 用户评价
        "list": re.compile("<li.*?>.*?</li>",re.S),    # 每个商店的li标签
        
        #以下为最终提取获得的数据
        "shop_name":re.compile('<div class=\"tit\".*?>.*?<h4>(.*?)</h4>.*?</div>',re.S), # 商店名称
        "shop_url": re.compile('<div class=\"tit\".*?>.*?href=\"(http://www.dianping.com/shop/\d*)\".*?</div>',re.S), # 商店的url
        "shop_type": re.compile("<span class=\"tag\">(.*?)</span>"),    # 商店类型
        "address": re.compile("<span class=\"addr\">(.*?)</span>"),    # 商店地址
        "money": re.compile("人均.*?<b>(.*?)</b>",re.S),    # 商店人均消费价格
        "star": re.compile("<span class=\"sml-rank-stars sml-str(\d\d?)\".*?>"), # 商店评分
        "username": re.compile('<p class=\"name\" ?>.*?<a.*?>(.*?)</a>.*?</p>',re.S),  #用户名称
        "img": re.compile('<div class=\"pic\" ?>.*?<img.*?src=\"(.*?)\"', re.S),# 商店图片
        "comment": re.compile('<div class=\"J_brief-cont\" ?>(.*?)</div>',re.S),# 用户评价
        "user_star":re.compile('class=\"item-rank-rst irr-star(\d\d?)\"') # 用户给商店评的星
        }
        
        
DZ_headers = {'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0',
           'Connection':'keep-alive','Accept-Language':'en-US,en;q=0.5',
           'Accept-Encoding':'gzip, deflate',
           'Cookie':'_hc.v=f123afde-9073-18d8-5811-05cb861eccab.1497540742; __utma=1.285283131.1497540742.1497540742.1504948182.2; __utmz=1.1497540742.1.1.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; wed_user_path=2784|0;_lxsdk_s=15e6e7d00d3-02-076-f5d%7C%7C64; s_ViewType=10; aburl=1; _lx_utm=utm_source%3Dbaidu%26utm_medium%3Dorganic; _lxsdk_cuid=15e6e7d00cfc8-04026bdcbcb8ae8-71206751-100200-15e6e7d00d0c8; _lxsdk=15e6e7d00cfc8-04026bdcbcb8ae8-71206751-100200-15e6e7d00d0c8; __mta=216783401.1505092288560.1505094427929.1505095299022.4; PHOENIX_ID=0a010725-15e6e9e52c8-f083785'}
