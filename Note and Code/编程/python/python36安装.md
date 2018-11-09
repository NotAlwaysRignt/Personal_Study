### 安装python3.6 
到官网下载源代码
解压，
```bash
./configure --prefix=mydir --enable-shared
make
make install
```
其中 mydir 是我指定的自定义目录,所有手动安装的软件放在该目录下
安装完后 mydir 目录下会出现 bin lib share 几个目录, 这样可执行文件python3就会出现在 mydir 的bin目录下， 相关的库文件也会在 mydir 的lib文件夹里
**为了支持一些软件如vim的功能，在使用./configure时加上 --enable-shared**
