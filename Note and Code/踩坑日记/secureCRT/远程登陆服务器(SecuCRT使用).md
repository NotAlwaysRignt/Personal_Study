### 跳板机的作用  
为了提供更加安全的环境，应该先登陆到跳板机（每个事业群都有自己的跳板机），然后再从跳板机这个Linux 环境中远程登陆到 工作组的开发环境  

### 服务器登陆  
**注意事项，不要在服务器中用yum安装软件，因为所有组成员的软件是共用的，安装后会影响其他成员**
打开secureCRT 可以用这个软件远程登陆到跳板机  

#### SecureCRT下载
http://ywts.oa.com
输入账号密码登录

#### 使用SecureCRT登陆 

SecureCRT 登陆跳板机的使用方法
http://km.oa.com/group/650/articles/show/144238?kmref=search&from_page=1&no=1  

### 配置SecureCRT
##### 上传,下载文件路径  
使用 sz rz 命令可以在 secureCRT 上把 Windows上的文件上传到 SecureCRT远程服务器中(rz)，也可以从服务器上把文件下载下来(sz)
当使用rz时，会从一个默认路径开始查找，如果这个路径是不常用的路径，那么每次都要繁琐地跳转目录，另外执行sz命令时也会传送到一个默认目录上，我们也希望将文件传送到指定目录中，而不用再拷贝  
设置方法
options->session options ->Terminal->Xmodem/Zmodem
##### rz 使用注意事项
从跳板机登录到开发机用户或者测试机以后，使用 rz 接受压缩文件会到处开发机或者测试机拒绝，一个解决的办法是加参数，加 -ybe
```bash
rz -ybe  #这样接受文件就不会被拒绝了
```



### 配置SecureCRT
SecureCRT 的相关配置可以参考以下两篇文章，可以进行美化界面等工作
配置可以在 options> session options 或 global options 处使用
https://blog.csdn.net/u014079773/article/details/69257552
##### 背景色的配置
https://blog.csdn.net/u010031673/article/details/51130889
非常好的一篇配色文章，强烈建议按照其设置来配置  
粗体颜色和细体颜色就是对应终端显示粗体字和细体字时的颜色，教程里有些颜色并没有进一步配置，如果想改我们也可以自己对着调色板改，一般来说把颜色调浅些对眼睛会舒服一些

#### 进入服务器以后
服务器 ip 为 10.123.10.10 密码为36000
```bash
ssh 10.123.10.10 36000
# 密码就是我的英文名
```

如果上面操作失败了，则改用
```bash
ssh chaoranxu@10.123.10.10
#密码就是我的英文名chaoranxu
```

### backspace键失灵
有时候backspace会无法起到删除的作用，而是显示 ^H ^H
解决办法：
```bash
Options–>Session Options–>Terminal–>Emulation–>Mapped Keys(中文：选项–>回话选项–>终端–>仿真–>映射键)
```
然后：
```
选中复选框  （Backspace sends delete、  Delete sends backspace）
```