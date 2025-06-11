# VPS搭建
** GPT 和云服务器上的搜索是好帮手，不懂就问**。
## 1. 服务器购买
在腾讯云上选购服务器，2核2GB的最基础版已经足够用，最主要是带宽，6MB已经足够快
## 2. 域名购买
域名的作用是支持客户端和服务器使用SSL连接。
客户端和服务端的加密算法，诸如VMESS，VLESS，属于应用层的加密。SSL是传输层的加密，可以增强抗封锁的能力。
在云服务器上购买域名即可，SSL绑定这些都是配套的

## 3. SSL绑定
绑定过程即由CA认证你对域名所有权，通常是由CA给定一串带时效性的密钥，要求你配置到域名的解析上，再由CA去请求域名解析，如果对上了，说明你确实拥有这个域名。
确认后，即签发SSL证书。之后我们就可以下载SSL的证书和密钥部署到服务器上。

客户端请求SSL服务端的流程可以概括为：
1. 客户端通过DNS解析域名请求服务器
2. 服务器返回SSL证书和密钥
3. 客户端拿着SSL证书（有CA签发的密钥）去对应的CA确认是否该CA签发的（防止这个SSL请求被中间人篡改了），如果是的话，由于SSL又绑定了域名，既可以自己确实请求到了域名对应的服务器回包。
4. 建立SSL连接

### 为什么SSL依赖域名
SSL 依赖 CA（证书机构）签发，IP端口理论上可行，但CA通常不会这么做。依赖CA是为了防止中间人攻击，即使服务端返回了一个SSL，可能也是被攻击者劫持返回的，如果SSL上绑定的域名由CA进行了认证，那说明这个SSL确实就是属于这个域名的。

## 4. Nginx安装
V2ray 通常与 Nginx 使用，而不是直接将V2ray暴露支连。客户端连接 Nginx，而V2ray通过本地端口再与Nginx交互。原因如下：
1. Nginx 有优秀的网络连接管理能力，可以轻松地集成 SSL，支持多种传输层协议。
2. 同时可以通过URL路由映射到V2ray，再将另一些URL作为静态网页暴露外网，这样即使端口被扫描也不容易发现。

Nginx安装只要通过 `apt install nginx` 即可，我们要将前面申请的SSL证书部署到Nginx上，只要在管理台上将 SSL 证书的 crt，key文件下载下来，然后上传到云服务器上，在Nginx的配置中指向它们接口，非常简单。
这里可以搜索腾讯云文档，有详细的介绍。 [SSL 证书 Nginx 服务器 SSL 证书安装部署（Linux)](https://cloud.tencent.com/document/product/400/35244)
### 配置示例
`apt-install` 安装的Nginx，配置在`/etc/nginx/nginx.conf`
这里我将配置示例也贴到下面
```shell
user www-data;
worker_processes auto;
pid /run/nginx.pid;
include /etc/nginx/modules-enabled/*.conf;

events {
        worker_connections 768;
        # multi_accept on;
}

http {

        ##
        # Basic Settings
        ##

        sendfile on;
        tcp_nopush on;
        types_hash_max_size 2048;
        # server_tokens off;

        # server_names_hash_bucket_size 64;
        # server_name_in_redirect off;

        include /etc/nginx/mime.types;
        default_type application/octet-stream;

        ##
        # SSL Settings
        ##

        ssl_protocols TLSv1 TLSv1.1 TLSv1.2 TLSv1.3; # Dropping SSLv3, ref: POODLE
        ssl_prefer_server_ciphers on;

        ##
        # Logging Settings
        ##

        access_log /var/log/nginx/access.log;
        error_log /var/log/nginx/error.log;

        ##
        # Gzip Settings
        ##

        gzip on;

        include /etc/nginx/conf.d/*.conf;
        include /etc/nginx/sites-enabled/*;

        server {
         #SSL 默认访问端口号为 443
         listen 443 ssl;
         #请填写绑定证书的域名
         server_name xxx.com;
         #请填写证书文件的相对路径或绝对路径
         ssl_certificate  your_ssl.crt;
         #请填写私钥文件的相对路径或绝对路径
         ssl_certificate_key your_ssl.key;
         ssl_session_timeout 5m;
         #请按照以下套件配置，配置加密套件，写法遵循 openssl 标准。
         ssl_ciphers ECDHE-RSA-AES128-GCM-SHA256:ECDHE:ECDH:AES:HIGH:!NULL:!aNULL:!MD5:!ADH:!RC4;
         #请按照以下协议配置
         ssl_protocols TLSv1.2 TLSv1.3;
         ssl_prefer_server_ciphers on;
         location / {
           #网站主页路径。此路径仅供参考，具体请您按照实际目录操作。
           #例如，您的网站主页在 Nginx 服务器的 /etc/www 目录下，则请修改 root 后面的 html 为 /etc/www。
           root /var/www/html;
           index index.html index.htm;
         }
         # V2Ray WebSocket 入口
         location /ray {
           proxy_redirect off;
           proxy_pass http://127.0.0.1:10000;
           proxy_http_version 1.1;
           proxy_set_header Upgrade $http_upgrade;
           proxy_set_header Connection "upgrade";
           proxy_set_header Host $host;
           # 防止超时
           proxy_read_timeout 3600s;
           proxy_send_timeout 3600s;
         }
        }
        server {
         listen 80;
         #请填写绑定证书的域名
         server_name xxx.com;
         #把http的域名请求转成https
         return 301 https://$host$request_uri;
        }
}
```
上述配置中达到的效果是，我们可以通过`https://xxx.com`访问网站，代理地址在`xxx.com/ray`这个路径下

主要关注的是`http->server`里的配置，其他都是自带的，我们不需要改动
* `listen 443 ssl`：表示使用SSL协议，监听443端口，443是HTTPS的默认端口，通常云服务器的安全策略组会对其开放，这里调试的时候如果不通，**要确认是不是云服务器把这个端口屏蔽了**
* `server_name`:申请的域名
* `ssl_certificate`：SSL证书下载下来的 crt 证书文件，这个文件如果和配置文件在同一目录下，即`/etc/nginx/`下，则直接写文件名即可
* `ssl_certificate_key`：SSL证书下载下来的密钥文件，这个文件如果和配置文件在同一目录下，则直接写文件名
* `location /`: root表示访问`xxx.com/`时，默认到哪个目录下去寻找资源，这里指定了`/var/www/html`，表示会在这个目录下找文件，`index index.html index.htm`表示优先找index.html，找不到则找index.htm。这两个文件是要自己创建的，我们可以让AI给我们写个漂亮的H5首页，这里主要是用来调试，看看我们使用`https://xxx.com`访问的时候，是不是能访问到index.html，如果能，那么说明域名，SSL，到Nginx这整条链路都是调通了。同时也实现上面说的，将代理隐藏在网站中，更加不容易被发现和封锁
* `location /ray`是我们的核心转发配置，上面的`/ray`定义了代理转发，即如果客户端访问了`xxx.com/ray`则将其转发到`http://127.0.0.1:10000`端口，而我们的v2ray则通过监听10000端口，来实现代理功能，注意这里的端口要对上

完成上述配置后,使用`systemctl restart nginx` 重启服务器，然后用 `systemctl status nginx`看看是否成功启动~

## 5. V2ray 安装
### 服务器端
v2ray的官网被墙了，但github没有。
v2ray的所有项目，文档都在上面了，按需查找即可
[V2Fly · GitHub](https://github.com/v2fly/)
[V2ray各平台安装 GitHub](https://github.com/v2fly/v2fly-github-io/blob/master/docs/guide/install.md)

v2ray的安装需要root权限，而购买腾讯云服务器登录时，默认是普通用户，root用户单独需要创建,使用以下命令接口
```shell
sudo passwd root
```
[云服务器 登录和连接实例问题_腾讯云](https://cloud.tencent.com/document/product/213/17278?from=console_top_search)

#### 服务器端配置
安装成功后默认配置在`/usr/local/etc/v2ray/config.json`中，与上面所属的Nginx搭配
```json
{
"log": {
  "access": "/var/log/v2ray/access.log",
  "error": "/var/log/v2ray/error.log",
  "loglevel": "error"
},

  "inbounds": [
    {
      "port": 10000,
      "listen": "127.0.0.1",
      "protocol": "vless",
      "settings": {
        "clients": [
          {
            "id": "your-uuid", //用 uuidgen 这个命令生成，客户端配置和服务端配置要一致
            "level": 0
          }
        ],
        "decryption": "none"
      },
      "streamSettings": {
        "network": "ws",
        "wsSettings": {
          "path": "/ray"
        }
      }
    }
  ],
  "outbounds": [
    {
      "protocol": "freedom",
      "settings": {}
    }
  ]
}
```
以下是配置的描述
* json 语法不支持注释，v2ray支持
* 初次搭建时，建议配置log，并设置level 为 `info`,主要是用来调试看是否客户端和服务器成功连接，等调通后再设置为error
* `inbounds`表示数据流入v2ray，作为服务端，对应的是nginx的数据，而nginx的数据又来自客户端，因此这里应该是一个加密协议的数据流，协议的配置需要和客户端的对上，才能成功进行解密交互。port与nginx的端口要对应上。protocol是应用层协议，可以支持vmess，vless等，vless的抗封锁能力是最好的，同时这里要与客户端的配置对应上。streamSettings 对应是传输层协议，ws是websocket的意思/ray是websocket的路径，这些都要和客户端对应上。
* `outbounds`表示数据流出v2ray，作为服务端，对应的是客户端请求网站经过v2ray后又转发到外网，这里因为我们只是透传，就不需要再做任何加密了，因此使用freedom。

### 客户端
v2ray官网提供的是一个内核，许多客户端在其内核上做了UI的二次开发，配套已经很成熟，建议使用二次开发的V2ray客户端。
#### windows 客户端
Windows 建议使用 V2rayN  [Releases · 2dust/v2rayN](https://github.com/2dust/v2rayn/releases)
下载`v2rayN-windows-64.zip` 版本即可(这里不知道为什么我的windos10安装另外几个版本用不了，运行时会要求你安装 `.net desktop runtime`(因为客户端是NET编写的，依赖运行环境），按要求下载即可
[下载 .NET 8.0 Desktop Runtime (v8.0.15) - Windows x64 Installer](https://dotnet.microsoft.com/zh-cn/download/dotnet/thank-you/runtime-desktop-8.0.15-windows-x64-installer?cid=getdotnetcore)
#####  配置
点击`配置文件`->`添加【VLESS】配置文件`（因为服务器上配置的协议是VLESS，协议是对应的）
配置项中阐释如下：
###### 配置文件
* 别名：随意取，就是自己辨认的名字
* 地址：申请的域名，xxx.com，根域名即可，不需要带路径
* 端口: Nginx对外暴露的端口，比如根据上面的配置，那么就是443
* 用户ID：和v2ray 服务端配置的ID要一致，这里其实也起到了密钥的效果，不然其他人也可以连我们的VPS了
* 流控 flow:**注意这里啥也不要选**，因为服务端没有配，如果设置了，服务端的`access.log`会报错
 ```shell
 rejected  proxy/vless/encoding: failed to read request version > websocket: close 1000 (normal)
 ```
 ###### 底层传输方式
 * 传输协议： ws，和v2ray 服务端配置一致
 * 伪装类型：只有none选项
 * 伪装域名：服务端没有配置，客户端也不需要配置
 * 路径: `/ray` 和nginx以及v2ray上的配置是一致的
 * 传输安全层: tls
 * SNI,fingerprint,Alpn 均不需要填
 * 跳过证书验证（allowInsecure）：因为我们是官方申请的证书，因此选 false，防止中间人攻击。

##### 开启客户端
以上配置创建完毕后，我们可以右击配置，选择`一键多线程测试延迟和速度`，如果链路是通的，则下方会显示延迟和服务器 IP，服务器上的access.log也会显示被访问的记录(日志设置info)。延迟显示-1ms则表示不通

确认连通后，我们在`系统代理`上可以看到4种模式，他们对应的含义如下（GPT给出）
|选项|含义|作用场景|
|:-:|:-:|:-:|
|清除系统代理|关闭代理时清空系统代理设置|关闭代理，恢复无代理状态|
|自动配置系统代理|启动代理时自动设置系统代理指向 V2rayN 本地代理端口|需要全局代理，自动生效|
|不改变系统代理|启动或关闭时不修改系统代理设置|用户自行管理系统代理|
|PAC模式|设置系统代理为 PAC 文件地址，实现智能分流|需要分流代理，部分流量走代理|

默认是`清除系统代理`，即我们的网络请求不会通过代理，我们希望代理生效，简单可以直接使用`PAC 模式`。接下来去访问google，就可以验证效果啦

`路由配置`表示我们访问不同网站域名时的规则，我们可以屏蔽一些网站的流量，也可以设置指定网站不走代理（比如中国境内网站就不要走代理，可以提升网络速度），V2rayN默认已经配置好了一套规则`V3-绕过大陆(whitelist)`，直接使用默认的即可，如果需要修改，也可以在`设置`->`路由设置`处修改和查看，v2ray 提供了一些通配符，比如`geosite:cn`表示中国大陆域名，·`geoip:cn`表示中国大陆IP，direct表示直连，proxy则表示走代理，所以配置会很方便，更多规则可以看官网和GPT

启用TUN保持关闭，这个功能GPT说是更底层的IP劫持，没有试过，开启后似乎会有问题，暂不探究

#### 安卓客户端
android 建议使用 V2rayNG [Releases · 2dust/v2rayNG · GitHub](https://github.com/2dust/v2rayNG/releases)
#####  配置
配置的可视化界面Windows是一样的，配置完后点击测试，如果成功显示延时，则说明连接上了，就可以开启服务器使用了