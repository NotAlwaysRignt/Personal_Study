> 测试机里面常常需要多个终端窗口,比如要用 gdb 调试服务器时就需要挂住几个进程才能调试,这导致要开很多个session,tmux就是为了解决这个产生的! 测试机同样不连外网,所以不得不源码进行安装,同时要保证不影响其他人的使用

### 编译安装libevent
下载libevent源码，编译生成libevent库文件
```bash
./configure --prefix=/home/user_00/chaoranxu/usr/
make
make install
```
这样 /home/user_00/chaoranxu/usr/ 下就会出现 lib share bin include 这些目录,其中 include 目录中存放有libevent相关的头文件,lib目录存放了 libevent 编译生成的库文件

这里有个小插曲,因为测试机上没有make,我是手动安装了一个,又为了防止污染其他人,于是我在自己的目录下创建了一个.bashrc文件,用
```bash
alias make=~/chaoranxu/app/make-3.81/make 
```
来找到我的make,这可以确保自己的make被正确启动,但问题是在 makefile 执行过程中,使用alias 的命令是不会被 make 认出来的,于是我又添加了如下一步
```bash
export PATH=~/chaoranxu/app/make-3.81/:$PATH
```
把make也添加到PATH变量中,这样才解决了问题


### 编译 tmux
下载 tmux 源码包,tmux 在编译时默认到系统路径下找寻 libevent,如果找不到则会在 ./configure 这一步出错显示,
libevent not found,所以我们在调用 ./configure时就应该指定编译时头文件和库文件的位置
```bash
./configure --prefix=/home/user_00/chaoranxu/usr/ --enable-static CFLAGS=-I/home/user_00/chaoranxu/usr/include/ LDFLAGS=-L/home/user_00/chaoranxu/usr/lib
```
**CLFAGS=-I 后面的路径不要用~，另外，路径应该紧跟在-I后面，中间不要有空格， --enalble-static选项必须启动**
CFLAGS 和 LDFLAGS 就是编译时的选项,使用 ./configure --help 可以知道这两个变量是可设置的

### 运行tmux
在运行 tmux 时,报错
```bash
 error while loading shared libraries: libevent-2.1.so.6:│
 cannot open shared object file: No such file or directory
```
tmux在动态链接 libevent时找不到libevent的位置
一个简单的解决办法是
进入/home/user_00/chaoranxu/usr/lib
即进入到 libevent 的动态库文件所在的位置,进入到这个位置后,调用tmux,启动成功,之后我们就可以切割窗口,到处跳转啦!

还有一个办法是立相应的symbol link
32bit系统
```bash
ln -s /usr/local/lib/libevent-2.0.so.5 /usr/lib/libevent-2.0.so.5
```
64bit系统
```bash
ln -s /usr/local/lib/libevent-2.0.so.5 /usr/lib64/libevent-2.0.so.5
```
显然,这会污染到整个系统,因为我们组内还是多人共用一个用户的,所以能不采用这个办法就不采用这个办法吧