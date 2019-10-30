package main

import (
  "fmt"
	"github.com/antlr/antlr4/runtime/Go/antlr"
  "go_parser/parser" // antlr 是go.mod中module的名称
  "os"
)

type GolangFileParserListener struct {
  *parser.BaseGolangParserListener // 这里要继承golangparser_base_listener.go 里面默认实现的方法
}

// func (this *GolangFileParserListener) EnterEveryRule(ctx antlr.ParserRuleContext) {
//   fmt.Println("rule:", ctx.GetText())
// }

// 参照 golangparser_base_listener.go 里面的方法, 去做具体的实现
func (this *GolangFileParserListener) ExitFunctionDecl(ctx *parser.FunctionDeclContext) {
  // fmt.Println("Function Name\t\t:", ctx.GetText())
  fmt.Println("Function Name\t\t:", ctx.IDENTIFIER())
}

func FileParser() {
  fmt.Println("***************FILE PARSER***************")
  // 我们可以用 ./programer filename的形式运行程序, 如./test.out hello.txt, 程序会读取 hello.txt的内容进行解析
	input, err := antlr.NewFileStream(os.Args[1])
  if err != nil {
    fmt.Println(err.Error())
    return
  }

	lexer := parser.NewGolangLexer(input)
	stream := antlr.NewCommonTokenStream(lexer,0)
	p := parser.NewGolangParser(stream)
	// p.AddErrorListener(antlr.NewDiagnosticErrorListener(true))  // 如果加上这句, 就会在运行时打印出许多信息
	p.BuildParseTrees = true
	tree := p.SourceFile()
  // 这里我们也可以用 tree := p.FunctionDecl() 等规则, 区别:
  // 使用p.FunctionDecl()则会去判断读取的字符串是否符合FunctionDecl的规则, 如果读取字符串中有 package 等关键字, 则会解析失败, 因为FunctionDecl里没有 package 的规则解析
  // 其他标识符如methodDecl等使用方法同理
  listener := &GolangFileParserListener{}
	antlr.ParseTreeWalkerDefault.Walk(listener, tree)
}

func main() {
   FileParser()
}
