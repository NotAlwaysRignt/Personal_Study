我们下载源码包要进行编译安装时，并不是所有的源码包中都已经放置了configure文件，但往往会放置 Makefile.in，configure.ac，这些文件，我们需要用autotools借助这些文件来生成 configure文件，然后再三步走(./configure & make & make install)

### 使用autotools
参考文章：http://www.51cos.com/?p=1649

### 简单的方式：

如果拿到的工程文件中，没有Makefile文件，而只有configure.in和Makefile.am文件，我们是不能够直接进行编译的，必须在UBUNTU等Linux系统下，根据configure.in和Makefile.am文件生成编译所需的Makefile文件。具体操作步骤如下：
##### 1、执行autoscan，然后执行aclocal，产生aclocal.m4文件
aclocal是一个perl 脚本程序，它的定义是：“aclocal – create aclocal.m4 by scanning configure.ac”。 aclocal根据configure.in文件的内容，自动生成aclocal.m4文件。而aclocal.m4文件中，包含了生成configure文件所必须的宏。
##### 2、执行autoconf，生成configure文件
autoconf会根据configure.in和aclocal.m4文件，生成configure文件。其实，autoconf就是把configure.in和aclocal.m4文件中定义的内容， 变成检查系统特性、环境变量、软件必须的参数的shell脚本。
##### 3、执行automake命令(建议加--add-missing)，产生Makefile.in
具体命令为：
```bash
automake -–add-missing
```
执行时要**先检查当前目录下是否有一个名为etc的目录**，如果没有就创建一个，否则报错 automake会自动进入到然后生成一些配置文件以及目录  
如果已经存在etc目录，并且已经放置了文件，这时要执行automake，先把这个etc目录清空，然后再执行automake -–add-missing，否则会报错，导致无法生成automake.in文件
automake会根据Makefile.am文件产生一些文件，包含最重要的Makefile.in。前面所生成的configure，会根据Makefile.in文件，来生成最终的Makefile文件。
##### 4、执行configure命令，生成Makefile文件
这样，就产生了编译所需要的Makefile文件。运行make，即可编译。


### 复杂的方式
复杂点的方式：

为一个项目源文件生成makefile并make的步骤如下：

操作在包含源文件的项目目录下进行。

1. autoscan，生成文件configure.scan
2. configure.scan，改名为configure.in
3. autoheader，生成文件configure.h.in（现在一般改为configure.ac）
4. libtoolize，生成ltmain.sh
5. allocal，生成aclocal.m4
6. autoconf，生成configure
7. automake，生成makefile.in，每个包含makefile.am的子目录都生成makefile.in。automake -a选项可以补齐文件config.guess，config.sub，install-sh，missing，depcomp
8. ./configure，生成config.status，config.h，makefile
9. make，生成中间文件对象文件，库文件，最后生成可执行文件
10. make install，相应的可执行文件，库文件，头文件拷贝到系统相应位置。


### configure运行

configure是一个shell脚本文件，由autoconf生成，它自动为源码包配置编译连接选项，适应不同的硬件平台和POSIX操作系统，输出所需要的Makefile。

configure主管检查你的系统，把结果存放到config.status中，config.status根据它的检查结果实际执行正确的动作。

configure检查与系统相关的一系列变量，这些变量存储到文件config.status中，供makefile调用。这些变量包括编译连接时需要的程序，这些程序在系统中的位置（目录），调用这些程序的选项，比如编译器的目录，编译器的选项-g是否支持等。configure能猜出它运行的系统的规范名字cpu–vendor–os，它通过运行脚本文件config.guess输出变量uname来猜出。configure能识别很多系统名字的别名，它通过运行脚本文件config.sub把系统名字变成规范名字。

make运行

makefile.am对makefile的影响：它根据SUBDIRS = add sub让make递归进入每个子目录处理子目录的Makefile。根据main_LDADD = add/libadd.la sub/libsub.la为main连接libadd.la和libsub.la库。

configure.in对makefile的影响：

根据AC_PROG_LIBTOOL让libtool完成编译连接工作。

根据AC_CONFIG_HEADERS([config.h])只需传递预处理宏-DHAVE_CONFIG_H给编译器。

makefile中很多与系统相关的信息都是通过变量获取的，这些变量之前已经由configure检查好存放在config.status里面，预处理 宏存放在config.h里面。比如我们要用到的编译器CC，编译器选项CFLAGS等。makefile中的变量完成替换后，开始实际执行命令，它会递 归执行每一个子目录下的makefile，生成对象文件，连接库文件，最后连接成可执行文件。

交叉编译 Cross-compiling

Q：为别的平台编译可执行程序怎么做？

交叉编译就是在目前的平台上为别的目标平台生成可执行程序或库。可以在运行configure时通过–build，–host，–target参数实现交叉编译。

例如:

```bash
./configure --build=i686-pc-linux-gnu --host=m68k-coff
```

–build=build-type ：configure和compile软件包的系统类型。默认情况等于config.guess给出的系统类型

–host=host-type ：运行软件包的系统类型。默认情况等于build类型

–target=target-type ：很少用，默认情况等于host类型。

交叉编译时，如果编译器，连接器，汇编器名字不是以host_type为前缀，configure都会发出警告。
要搭建交叉变异环境，如交叉编译用的编译器，连接器，汇编器跟本地的不一样，一般以host_type为前缀，如arm-pc-linux-gcc。
安装目录

Q：make install时，文件都安装到哪里去了？

prefix：安装目录的前缀。默认情况下/usr/local

bindir：安装时拷贝可执行文件到此目录。默认情况下/usr/local/bin 。

includir：安装时拷贝头文件到此目录。默认情况下/usr/local/include 。

libdir：安装时拷贝库文件到此目录。默认情况下/usr/local/libs 。

定制自己的安装目录，可以–prefix 和 –exec-prefix 给configure。

例如：./configure –prefix=/usr 。