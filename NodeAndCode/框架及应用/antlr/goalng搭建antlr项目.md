## 项目介绍
这里编写一个可以简单解析go语言文件的项目,读取文件中的函数等, 这个项目放在了`project/go_parser`目录下

### 编写一个解析golang语言的文件
从`https://github.com/antlr/grammars-v4/tree/master/golang`中获取golang的g4文件

在项目跟目录下`mkdir parser`创建`parser`目录,命名为parser

`antlr4 -Dlanguage=Go GolangLexer.g4 GolangParser.g4`
`-Dlanguage=Go`表示生成的antlr词法和语法解析工具是用go文件来描述的
参数里要有`GolangLexer.g4`和`GolangParser.g4`两个g4文件,而不能只有一个

把`grammars-v4/golang/Go`目录下的两个go文件`golang_base_parser.go`和`golang_base_parser.go`也拷贝到`parser`目录下,这个是依赖文件,根据g4生成的go文件需要依赖它们


### 修改生成的go文件
先上结论,如果不修改生成的go文件,一会生成`undefined`错误,二会出现空指针panic,下面逐一讨论如何解决
#### undefined问题解决
完成自己的编译程序后,直接编译文件会报错
```bash
parser/golang_parser.go|9747 col 8| undefined: noTerminatorAfterParams
parser/golang_parser.go|9996 col 8| undefined: noTerminatorAfterParams
parser/golang_parser.go|12395 col 8| undefined: noTerminatorBetween
parser/golang_parser.go|14472 col 8| undefined: lineTerminatorAhead
parser/golang_parser.go|14480 col 8| undefined: closeBrace
parser/golang_parser.go|14541 col 10| undefined: noTerminatorAfterParams
parser/golang_parser.go|14551 col 10| undefined: noTerminatorAfterParams
parser/golang_parser.go|14561 col 10| undefined: noTerminatorBetween
parser/golang_parser.go|14603 col 10| undefined: lineTerminatorAhead
parser/golang_parser.go|14606 col 10| undefined: closeBrace
```
都是未定义的错误,我们可以使用`grep`查看这些未定义变量所在的文件,如`grep -rin closeBrace ./`
可以发现`closeBrace`在`golang_parser.go`中出现,另外也在`golang_base_parser.go`中出现,前面提到过
`golang_base_parser.go`和`golang_base_parser.go`是项目中自带的文件,这里我们需要修改这些`undefined`的变量,使这些变量指向`golang_base_parser.go`和`golang_base_parser.go`文件中的实现

修改的方法如下:
比如我们要修改`undefined: noTerminatorAfterParams`的问题,这个错误出现的位置
```go
func (p *GolangParser) MethodSpec() (localctx IMethodSpecContext) {
    // ......
    if !(noTerminatorAfterParams(2)) {
        panic(antlr.NewFailedPredicateException(p, "noTerminatorAfterParams(2)", ""))
    }
    // ......
}
```
`noTerminatorAfterParams`函数出现在`GolangParser`的方法中,阅读代码(这也是没办法的事情,出了问题只能这样调试),发现
```golang
 type GolangParser struct {                                                                                                                                                       
    GoBaseParser
  }
```
而`noTerminatorAfterParams`的实现在`GoBaseParser`中出现
所以`GolangParser`里面要调用`noTerminatorAfterParams`方法,只要将上面的`noTerminatorAfterParams(2)`改为`p.noTerminatorAfterParams(2)`即可
同理,其他`undefined`的变量,逐个更改(前面加上`p.`)使其能够正确调用到Base里面的方法,最终可以编译通过

#### 空指针 panic 错误解决
通过上面的代码修改,代码可以顺利编译,但在运行时会出现`panic`
报错出现在函数`NewGolangLexer`,解决这个nil错误的办法也只能去阅读代码
```go
func NewGolangLexer(input antlr.CharStream) *GolangLexer {
     
  l := new(GolangLexer)
     
  l.BaseLexer = antlr.NewBaseLexer(input)
  l.Interpreter = antlr.NewLexerATNSimulator(l, lexerAtn, lexerDecisionToDFA, antlr.NewPredictionContextCache())
     
  l.channelNames = lexerChannelNames
  l.modeNames = lexerModeNames
  l.RuleNames = lexerRuleNames
  l.LiteralNames = lexerLiteralNames                                                                                                                                               
  l.SymbolicNames = lexerSymbolicNames
  l.GrammarFileName = "GolangLexer.g4"
  // TODO: l.EOF = antlr.TokenEOF
     
  return l
}  
```
经过调试发现在出错在`l.BaseLexer = antlr.NewBaseLexer(input)`上
查看`GolangLexer`的定义
```go
type GolangLexer struct {                                                                                                                                                          
  *GoBaseLexer                               
  channelNames []string
  modeNames    []string
  // TODO: EOF string                        
}
```
`GolangLexer`里面组合了`*GoBaseLexer`类型,而`l.BaseLexer`中的`BaseLexer`成员是在`GoBaseLexer`中定义的
由此可以知道panic的原因,当使用`l := new(GolangLexer)`时,拿到的是一个`{nil,[],[]}`对象,这时如果对`l.BaseLexer`赋值,相当于对`nil`成员里的变量复制,这就会导致`panic`
解决办法
将`type GolangLexer struct`中的`*GoBaseLexer`改为`GoBaseLexer`,即不用指针类型,至此代码运行可以通过

### 总结
g4文件生成go文件后还是有要修改的地方,我理解这个应该是一个bug, 正常情况下应该不需要修改就可以通过,或许是自己有的地方没有设置对,或许是antlr不完善的地方,希望后续可以改进