### 安装python3.6 
到官网下载源代码
解压，
```bash
./configure --prefix=mydir --enable-shared
make
make install
```
其中 mydir 是我指定的自定义目录,所有手动安装的软件放在该目录下
安装完后 mydir 目录下会出现 bin lib share 几个目录, 这样可执行文件python3就会出现在 mydir 的bin目录下， 相关的库文件也会在 mydir 的 lib 文件夹里
**为了支持一些软件如vim的功能，在使用./configure时加上 --enable-shared**


### python3.7与前面版本的区别
python3.7不同于前面的版本,它还需要依赖`libffi-devel`
因此在源码编译安装python3.7前,使用`apt-get install libffi-devel`(或 yum install)

### 动态库地址添加
我们将 python 安装到自定义目录下，然后在`.bashrc`中将python的路径添加到`PATH`路径后,运行命令`python3`
会发现如下报错
```bash
python3: error while loading shared libraries: libpython3.7m.so.1.0: cannot open shared object file: No such file or directory
```
根据提示信息可知动态库`libpython3.7m.so.1.0`找不到,原因是`libpython3.7m.so.1.0`在自定义安装目录下,系统在查找动态库时不知道这个自定义地址,解决办法当然是添加这个地址

首先要找到`libpython3.7m.so.1.0`的位置:
`find / -name 'libpython3.7m.so.1.0'`

之后在目录 `/etc/ld.so.conf.d` 下，建立 `python3.conf`,并在 `python3.conf` 中加入`libpython3.7m.so.1.0`的位置,比如加入一行
```bash
/usr/local/lib/
```
接着再运行`ldconfig`使配置生效即可

如果在配置后还是找不到, 那么建议去了解下`/etc/ld.so.conf.d`的配置机制,可能在有些平台下配置方式不同
