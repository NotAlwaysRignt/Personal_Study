官网地址: https://github.com/jmcnamara/libxlsxwriter

### 简介
libxlsxwriter 是一个C 语言程序库用于创建和操作 Excel 的 XLSX 文件
安装 libxlsxwriter 的依赖库为 zlib
协议: FreeBSD

### 安装
从 github 上下载 zip,然后解压
这里使用cmake 的方法, CMakeList.txt的注释中介绍了两种方法,这里介绍一种
在 CMakeList.txt 所在的目录下(项目根目录)
```bash
mkdir mybuild
cd mybuild

#这一步会检查zlib是否安装
cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release

#生成静态库 libxlsxwriter.a 到当前执行 cmake 时所在的路径下(这里为 mybuild ), 上面把 -DBUILD_TESTS=ON 打开了,这一步还会生成 xlsxwriter_unit,用于测试demo是否正常运行
make

# 运行 xlsxwriter_unit 检查是否通过测试,如果通过测试则说明,也可以手动 ./xlsxwriter_unit 运行测试
make test

#可以不执行,这一步是把当前 mybuild 目录下的 libxlsxwriter.a 拷贝到系统库的路径下
make install
```

### 使用方法
1. 头文件: 将项目的 include 作为头文件路径包含进来
2. 库文件: 链接 libxlsxwriter.a ,即把 libxlsxwriter.a 路径用-L 包含进来,并用 -l 链接上

```bash
# makefile 示例
INC+=...\
     -I/mypath/libxlsxwriter-master/include\

LIB+=...\
     -L/mypath/libxlsxwriter-master/lib -lxlsxwriter\
```

### 使用示例
解压获得 libxlsxwriter-master 后,可以在根目录下看到 examples 目录和 test 目录两个文件夹,里面有大量的示例可供参考,其中 examaple 中的示例配有丰富的注释,并可以编译为可执行文件编译运行查看结果,非常值得一看