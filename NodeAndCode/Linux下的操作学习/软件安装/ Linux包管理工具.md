### 常用工具  
* rpm
* dpkg
* apt-get
* yum


### 包管理工具的关系  
RPM:　Red-Hat Package Manage, 它是 RetHat Linux 下的包管理工具
DPKG: Debian Package, Debian 下的包管理工具  
RPM 和 DPKG的作用是基本相同的,就是可以方便地对一个软件包进行安装,删除,更新,从互联网上获取相应版本下的软件包  

它们也有不足,就是当要安装的软件包依赖与其它软件包是,它没办法自动去安装这些依赖的软件包,同时如果某个软件包要升级,它也不会去解决这些软件包的依赖关系,而 yum 和 apt-get则是为了解决这些问题而存在的,相当于对RPM和DPKG的封装

yum: 对 RPM 的封装,用于 RET-HAT Linux  
apt-get:对 dpkg的封装,用于 Debian  