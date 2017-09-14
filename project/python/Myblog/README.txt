为了在下载代码后能顺利运行，需要在原有的源代码基础上进行修改：

在 config.py文件中，需要对自己的mysql数据库进行相关的配置，
并对 SQLALCHEMY_DATABASE_URI 参数进行响应的设置

SECRET_KEY、WTF_CSRF_SECRET_KEY 设置为自己的密码

因为js代码中使用了高德地图api，所以为了更好地获得高德的服务，应该在需要 key值的地方填充
自己在申请到的key值
代码中需要填写自己key值的地方

app/templates/searchsite/map.html 找到"key=申请的key值"，将"申请的key值"替换为自己的key值
