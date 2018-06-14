#### 使用.deb包安装软件  
使用.deb包安装软件一般会经过如下步骤  
```bash
dpkg -i  XXX.deb  　　　# XXX为要安装.deb文件 
sudo apt-get update  #　更新源地址
sudo apt-get upgrade  #　升级所有已经安装的package
sudo apt-get install -f   　#　修复系统上不满足依赖的package
```  

apt-get 是对 dpkg  命令(意为 debian package) 进行的更高一级的封装, dpkg 可以从 .deb (意为debian)里获取远程仓库的信息,并到远程仓库去下载相应的软件,但是 dpkg 并不能解决包的依赖关系,即它不会去下载某个软件依赖的包  

关于以上这些命令的详细解释,请apt-get的相关用法
