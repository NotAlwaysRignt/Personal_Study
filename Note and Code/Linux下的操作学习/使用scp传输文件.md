使用scp可以实现两台linux 服务器上文件的传输
在使用src之前,需要在服务器上的安全组上作相关配置:

设置协议类型: RAP协议 SSH协议 自定义端口 
授权对象可以用 0.0.0.0/0 (任意ip), 也可以是指定ip,这里授权对象我填写自己的ip
具体设置见阿里文档 "允许公网远程登录到实例"部分

文件传输命令
```bash
scp ~/github/project/requirements.txt root@106.15.200.34:/srv/
```

如果要传送文件夹及其所有的子文件,则
`scp -r ~/github/project/doc root@xxx.xxx.xxx.xxx:/srv/`
传送指定文件夹下的所有文件
`scp -r ~/github/project/* root@xxx.xxx.xxx.xxx:/srv/`

如果服务器 SSH 端口是22,则用上述格式即可, scp后跟的是自己主机上的文件(应包含完整路径),
再后面跟的是该文件要发送到的服务器上的位置,应填完整路径(小心别弄错了)

接下来会让输入服务器上 root 用户的密码
root@106.15.200.34's password: 

结果:
```bash
requirements.txt                              100%  432     0.4KB/s   00:00
```

### 一个实用的技巧
假如有ABC三台机器, A 可以登录 B 和 C ,但是 B 和 C 是隔离的,我们要将 B 中文件传输到 C ,可以先将文件从 B 拷贝到A,再从A 发送到C
```bash
# 先 copy 到本地, -r 表示如果是目录,则递归拷贝
scp  -r $UserName@$RemoteHost:$RemotePath  $LocalPath
# 再从本地 copy 到远程
scp -r  $LocalPath $UserName@$RemoteHost:$RemotePath
```
往往需要输入密码登录,结合 expect, send 效果更佳
