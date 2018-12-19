下载boost 包,把官网的boost源码压缩包下载下来,解压的大盘`boost_1_68_0`
进入目录`cd boost_1_68_0`
### bootstrap.sh 指定编译选项
使用`./bootstrap.sh --help` 这里讲解几个注意的
* `--prefix=PREFIX` 选项
```bash
  --prefix=PREFIX           install Boost into the given PREFIX
                            [/usr/local]
```
可以指定编译好后的库的安装路径,比如首先使用`./bootstrap.sh --prefix=/mypath/mydir`指定最终目录 ,使用`./b2 toolset=gcc`编译,最后使用`./b2 install`安装时,会在/mypath/mydir(如果一开始没有创建mydir则会创建它) 目录下生成 include 和 lib 两个目录,里面存放的是相应的头文件可库文件

* `--with-libraries`和`--without-libraries`选项
   我们可以使用 `./bootstrap.sh --show-libraries` 查看boost有哪些库以及库的名称要怎么填写,这样我们才可以在 `--with-libraries` 指定要编译的指定库或在`--without-libraries` 指定不需要的库
```bash
  --with-libraries=list     build only a particular set of libraries,
                            describing using either a comma-separated list of
                            library names or "all"
                            [all]
  --without-libraries=list  build all libraries except the ones listed []
```
可见,如果不指定默认--with-librarie是安装所有的库
使用举例
```bash
./bootstrap.sh --with-libraries=program_options,filesystem,system
```

* `--with-toolset`可以指定特定版本的编译器,如`--with-toolset=gcc-4.4`,如果不指定,则会自动扫描默认编译器

### b2 开始编译
指定编译选项
```bash
./bootstrap.sh --prefix=/home/ubuntu/commlib/c-family/boost_1_68_0
```
prefix 指定boost安装到我指定的目录下

执行后可以看到输出结果,并在当前文件夹下生成了一个名为b2的可执行文件,其中有一句:
```bash
Bootstrapping is done. To build, run:

    ./b2
```
意思就是我们执行 b2 就开始安装了
我们可以先看看b2这个可执行文件提供了哪些选项
```bash
./b2 --help
```
之后开始编译安装
```bash
./b2  # 耐心等待编译
```
编译结束后会看到有下面这样的提示,是正常的,别看到fail就以为失败了
```bash
...failed updating 56 targets...
...skipped 6 targets...
...updated 1081 targets..
```

运行结束后,项目目录下会多一个`stage`目录,`cd stage/lib`可以看见里面存放了编译好的 boost 库
但这时还没有把 boost 库安装到指定目录下,执行
```bash
./b2 install
```

安装结束后有如下提示,别看到fail就以为失败了
```bash
...failed updating 54 targets...
...skipped 6 targets...
...updated 14828 targets...
```
这一步以后,我们会发现,前面指定 `--prefix=/home/ubuntu/commlib/c-family/boost_1_68_0` 目录下多了 `include` 和 `lib`两个目录
`include`放的是boost所有需要的头文件,`lib`里放的是boost 的库文件

**如果在执行`./bootstrap.sh`时没有添加 --prefix 选项,那么我们可以在执行bs时添加prefix选项,`./b2 install --prefix=/mydir`**
