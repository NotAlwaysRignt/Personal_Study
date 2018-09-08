与redis交互，只要将遵循redis协议的字符串发送给redis，就可以获得正确地redis回复了
我们只要构造 struct redisRequest，
最常用的有 eCmdType(请求类型) strKey， strValue
然后往这个redis里面填数据，调用pack函数，然后就可以获取遵循 redis协议的字符串了

#### 注意 expire 的用法
头文件有误导！ expire 要填充的不是strTime，而是strValue, strTime是没用的

##### Redis是区分大小写的！
redis 关键命令不区分大小写，但变量区分大小写！