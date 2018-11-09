这个例子展示了如何链接googletest
lib 和 gtest 目录与main.cpp在同一级目录下,这里不将它们拷贝到这个github仓库里了
编译过程:
```bash
mkdir mybuild 
cd mybuild
cmake ..
make
```
发现main.out 在根目录下(mybuild的上一级目录)
