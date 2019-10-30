## rules 编写
编写一个rules的常用套路

### 递归
递归是经常用到的套路,善于用递归表达规则

### 可变长参数
`TAG(,TAG)*`

### 关键字
关键字就是用在antlr中就是双引号括起来的字符

### 描述一个占位符(变量)
一般而言,我们在描述变量的时候,要做进一步细分,一条rules是不够的
每个类型对应一条rules

描述一个变量是基本功,在变量的基础上我们又会有更多更丰富的抽象

### 学会模仿
编程语言的设计很多时候都是相互模仿和借鉴.如果我们想要设计一款自己的语言,但有很多语法和其他编程语言是类似的
我们应该去阅读学习已有的 g4 文件, 现有语言的 g4 文件往往设计良好,很多语法规则都可以直接改改就复用了
不仅能给给我们灵感,还能帮助我们快速完成开发

### 使用标记树和`ParseTreeProperty`
标记树即在每个语法旁加`#YourTag`标签,类似注释,但生成函数时会生成带`YourTag`的相关函数,这将大大提高代码可读性
使用 ANTLR 默认提供了`ParseTreeProperty`类型, 让我们可以使用一个`ParseTreeProperty`实例作为一个类中的成员变量,所有函数都可以访问这个变量以存放语法树中每个节点的值,
这将使得在语法树中各个节点相互访问会非常方便,缺点就是每个节点的相关信息全都保存在一个变量里,消耗内存会比较大,但是如果要分析的文本不是非常庞大,那么可以不用在乎这点开销

`ParseTreeProperty` 本质就是提供了一个map的封装,如果我们生成的目标文件不是用 java 语言,比如用 go 语言,可能就没有`ParseTreeProperty`,这时可以用golang的 map 来存储 ctx 和 对应的值

### 多个g4文件,则在编译时需要都包含进来
假如我们编写了文件`CalcParser.g4`和`CalcLexer.g4`
以生成 golang 目标文件为例, 需要调用 `antlr4 -Dlanguage=Go CalcParser.g4 CalcLexer.g4`活
`antlr4 -Dlanguage=Go *.g4` 而不是只用 `antlr4 -Dlanguage=Go CalcParser.g4` 否则会失败

## 常见错误
### 多个g4文件,则在编译时需要都包含进来
假如我们编写了文件`CalcParser.g4`和`CalcLexer.g4`
以生成 golang 目标文件为例, 需要调用 `antlr4 -Dlanguage=Go CalcParser.g4 CalcLexer.g4` 
活`antlr4 -Dlanguage=Go *.g4`,而不是只用 `antlr4 -Dlanguage=Go CalcParser.g4` 否则会失败