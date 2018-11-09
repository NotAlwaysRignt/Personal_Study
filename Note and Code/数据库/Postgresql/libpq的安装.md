### libpq介绍
libpq 是操作 Postgresql 的 C 语言库,网上介绍 Postgresql 的教程不多,因此这里特作介绍

#### libpq 库的安装
进入官网 -> Download -> Source Code
进入如下网址
https://www.postgresql.org/ftp/source/
选择一个版本下载,我安装的是 9.3.24
这里下载的Postgresql源码不止是libpq的,也包括客户端程序等的代码

经典三步走战略,但是要加一些参数
```bash
##根据提示,Use --without-readline to disable readline support,我们组没有readline库,所以disable
./configure --prefix=你想安装的目录 --without-readline
make -C src/interfaces/   #libpq 在 src/interfaces 目录下
make -C src/interfaces/ install
```
安装好后就会发现 --prefix 指定的目录下多了 include lib 几个目录

使用时用 -I 选项指定 include 目录, -L选项 指定 lib 目录,并添加 -lpq
makefile示例如下
```makefile
-Imydir/libpq/include\
-Lmydir/libpq/lib -lpq\
```

在Cpp文件中,只要包含头文件 libpq-fe.h 即可
```cpp
#include "libpq-fe.h"
```

**如果make install 后安装发现安装不全,还可以采用如下方法**
在make成功后,libpq 库(libpq.a)就在 src/interfaces 目录下,头文件也在 src/interfaces 下,不过还有个头文件 postgre_ext.h 在src/include 目录下,拷贝出来即可
