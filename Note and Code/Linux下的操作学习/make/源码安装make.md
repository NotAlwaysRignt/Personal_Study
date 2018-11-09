> 因为机器上没有make,所以我只能使用源码编译安装了一个,我要把make安装到自己指定的目录下

想把 make 安装到自己指定的目录下,那么你就得使用 ./configure 命令,然后加--prefix 选项指定自己的目录,这就会导致一个问题,一般情况下安装软件都会用
```bash
./configure
make
make install
```
三步走战略,但是显然在安装make时测试机没有make,那么我们在./configure后该怎么办?
目录下有个Makefile文件,但我们执行make它会提示没有make 这个命令
##### 解决办法
第一步依然是要执行 ./configure,生成安装相关文件
```bash
./configure --prefix=mydir
```
这时我们会发现目录下除了Makefile 还会有一个 build.sh 文件,如果你稍微查看下内容会发下里面都是编译的命令,是的,这个文件也回调用 我们系统的编译器来编译这个工程,执行
```bash
sh build.sh
```
这样就会进行编译,编译好后,我们在当前目录下(build.sh所在目录)就可以看见make这个可执行文件
我们可以使用 alias 指向这个文件,这样就不会
比如在某个目录下创建一个 .bashrc,添加如下内容
```bash
alias make=~/filepath/make-3.81/make
```
然后使用 source bashrc 或 . .bashrc 使其生效,我把`.bashrc` 放在另外创建的目录里,需要生效时再启用,这样就不会导致别人登录这个用户时也用了我的环境了
