import requests
import re


class Browser:

    # default headers
    default_headers={'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0',
           'Connection':'keep-alive','Accept-Language':'en-US,en;q=0.5',
           'Accept-Encoding':'gzip, deflate'}

    def __init__(self, url, headers=default_headers):           
        # If "http" or "https" is not claimed at the beginning of url, add "http://"
        if re.match("https?",url):
            self.page = requests.get(url,headers=headers)
        else:
            url = "http://"+url
            self.page = requests.get(url,headers=headers)
    
    def get_text(self):
        if self.page.status_code == 200:
                return self.page.text
        else:
            return None
                
if __name__=="__main__":
    b = Browser("http://www.dianping.com/")
    print(b.get_text())
