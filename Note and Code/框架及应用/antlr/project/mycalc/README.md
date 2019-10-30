# antlr Golang示例
本示例展示了如何编写antlr g4 文件

### build
```bash
cd parser
antlr4 -Dlanguage=Go CalcParser.g4 CalcLexer
go build -o calc  # calc 即为最终的可执行程序
```
这里 antlr4 是 alias
```bash
alias antlr4='java -Xmx500M -cp "yourlib/antlr-4.7.1-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
```

### 运行
```bash
./calc
```

改变代码, 去掉`FileParser()`的注释, 可以读取文件中的代码
```bash
./calc hello.txt  # hello.txt是自定义的文件名, 在当前运行程序时的路径中寻找, 里面可以写加法运算式
```
注意, 程序只能解析加法, 而且不支持带括号, 对于文件解析, 文件里只能写一行数据
```bash
1+2 # 支持
1  # 支持
1+2+3 #支持
(1+2) 不支持
1*2不支持
```

### 关于代码
详细的注释和注意事项写在代码注释和 g4 文件中
