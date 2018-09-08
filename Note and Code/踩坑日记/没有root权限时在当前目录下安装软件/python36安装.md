### 安装python3.6 
到官网下载源代码
解压，
```bash
./configure --prefix=/data/home/chaoranxu/bin/
make
make install
```
这样可执行文件python3就会出现在 /data/home/chaoranxu/bin/ 的bin目录下， 相关的库文件也会在/data/home/chaoranxu/bin/ 的lib文件夹里
**为了支持一些软件如vim的功能，请在使用./configure时加上 --enable-shared**