### 介绍  
apt-get是 Ubuntu 操作系统的软件包安装工具,在 CentOs 下与之相对应的是 yum
### apt-get install
apt-get install 是安装软件常用命令
```bash
sudo apt-get install 软件的名字
```  
一般情况下,输入以上指令即可完成安装,某个软件可能还会依赖其它的库文件,如果在安装某个软件时也没有找到这些依赖, apt-get 会自动帮我们把这些依赖安装  
#### 工作过程(要了解) 
当使用apt-get install 安装软件时,apt-get会到 /etc/apt/sources.list 和 /etc/apt/sources.list.d 
这两个文件里去查看远程仓库的网址,即这两个文件里存放的是当前已知的远程仓库的 URL ,如果在这些仓库中找到了相应的软件,就进行安装  
从上面也可以知道,如果远程仓库里没有要安装的软件,那么就不可能安装成功.因此,有些软件如果在特定的远程仓库中,就需要我们添加其相应的远程仓库的列表  
许多软件会提供.deb 包,里面就包含了这个软件远程仓库的信息  
#### 使用.deb包安装软件  
使用.deb包安装软件一般会经过如下步骤  
```bash
sudo dpkg -i  XXX.deb  　　　# XXX为要安装.deb文件 
sudo apt-get update  #　更新源地址
sudo apt-get upgrade  #　升级所有已经安装的package
sudo apt-get install -f   　#　修复系统上不满足依赖的package
```
关于以上这些命令的解释，请仔细阅读本文档　　

####　sudo apt-get install -f　　
-f参数为--fix-broken的简写形式，主要作用是是修复依赖关系（depends），假如用户的系统上有某个package不满足依赖条件，这个命令就会自动修复，安装程序包所依赖的包。

#### 关于依赖(apt-get与dpkg的关系)  
apt-get 是对 dpkg  命令(意为 debian package) 进行的更高一级的封装, dpkg 可以从 .deb (意为debian)里获取远程仓库的信息,并到远程仓库去下载相应的软件,但是 dpkg 并不能解决包的依赖关系,即它不会去下载某个软件依赖的包


### 软件的更新 
软件更新常用以下命令(先后执行以下三步)
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade  
```
update 是更新 /etc/apt/sources.list 和 /etc/apt/sources.list.d 中列出的源的地址,这样才能获取到最新的软件包  
而upgrade 是升级已安装的所有软件包，升级之后的版本就是本地地址里的，因此，在执行 upgrade 之前一定要执行 update, 这样才能更新到最新的   
sudo apt-get upgrade  和 sudo apt-get dist-upgrade 两者的目标是一样的,即更新系统中所有安装了的软件

upgrade:系统将现有的Package升级,如果有相依性的问题,而此相依性需要安装其它新的
Package或影响到其它Package的相依性时,此Package就不会被升级,会保留下来. 
 
dist-upgrade:可以聪明的解决相依性的问题,如果有相依性问题,需要安装/移除新的Package,
就会试着去安装/移除它. 
(所以通常这个会被认为是有点风险的升级)  

### 软件的删除
删除用apt-get安装的软件
apt-get --purge remove 软件名
 删除软件及其配置文件

apt-get remove 软件名
删除已安装的软件包（保留配置文件），不会删除依赖软件包，且保留配置文件。
#### 注意事项  
删除依赖软件包是一个非常危险的操作,因为可能会有其它软件也依赖这些包,在这种情况下,删除依赖可能会导致操作系统上的其它软件不可用  
当然,有时删除某个软件后,其依赖的包可能也就没用了,但是很多情况下我们并不知道这种依赖关系,所以最好的做法就是不要删除依赖包

### 举例
安装python pip
sudo apt-get install python-pip  即可安装pip

用sudo apt install在本系统不行，不知道为什么


