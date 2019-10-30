package main

// antlr 解析四则运算加法示例, 该程序只能解析简单的不带括号的加法
// 使用 listener 的方式, 使得我们可以对 antlr 有一个基本的了解
import (
	"github.com/antlr/antlr4/runtime/Go/antlr"
	"antlr/parser" // antlr 是go.mod中module的名称
  "os"
	"fmt"
  "strconv"
)

// antlr生成的代码中默认会有 *base_listener.go 文件, 我们在定义自己的监听器时需要组合这个 baselistener, 它默认为
// 所有的 Enter Exit 等接口实现了默认方法(就是空方法), 如果我们不组合这个这个结构体
// 那么我们就需要实现所有生成的 listener 接口, 否则在程序运行时会因为没有找到具体的方法实现而抛出 panic
// 这里要组合 BaseCalcParserListener
// 对于四则运算类型, 我们可以用 antlr.Tree 类型来接收每个节点的 *Context 类型, 以存储遍历时获得的中间值, 这是非常重要的技巧
type TreeCalcListener struct {
	*parser.BaseCalcParserListener
   Values map[antlr.Tree]int // antlr.Tree是一个接口, 使用它可以接收各种 context 类型
}

func NewTreeCalcListener() *TreeCalcListener {
	return &TreeCalcListener{
    Values:make(map[antlr.Tree]int),
  }
}

func (this *TreeCalcListener) ExitADD(ctx *parser.ADDContext) {
  var (
    left int
    right int
    // err error
  )

  /* 注释得写法是错误的, 因为ctx.Expr(0).GetText()的格式可能为"123 + 456", 这样就不能用 Atoi 解析了
  // fmt.Println("Expr(0):", ctx.Expr(0).GetText(), "\tExpr(1):", ctx.Expr(1).GetText())
  // if left, err = strconv.Atoi(ctx.Expr(0).GetText()); err != nil {
  //   this.Values[ctx] = 0
  //   return
  // }
  // if right, err = strconv.Atoi(ctx.Expr(1).GetText());err != nil {
  //   this.Values[ctx] = 0
  //   return
  // }
  */

  // 程序能正常运行, 反向证明了listener会遍历到最小的单元, 然后逐层往上
  left = this.Values[ctx.Expr(0)]
  right = this.Values[ctx.Expr(1)]
  this.Values[ctx] = left + right
}

func (this *TreeCalcListener) ExitINT(ctx *parser.INTContext) {
  var (
    err error
    num int
  )

  if num, err = strconv.Atoi(ctx.GetText()); err != nil {
    num = 0
    return
  }
  this.Values[ctx] = num
}

func (this *TreeCalcListener) EnterEveryRule(ctx antlr.ParserRuleContext) {
  fmt.Println("enter node:", ctx.GetText())
}

// 读取文件并解析
func FileParser() {
  fmt.Println("***************FILE PARSER***************")
  // 我们可以用 ./programer filename的形式运行程序, 如./test.out hello.txt, 程序会读取 hello.txt的内容进行解析
	input, err := antlr.NewFileStream(os.Args[1])
  if err != nil {
    fmt.Println(err.Error())
    return
  }
	lexer := parser.NewCalcLexer(input)
	stream := antlr.NewCommonTokenStream(lexer,0)
	p := parser.NewCalcParser(stream)
	p.AddErrorListener(antlr.NewDiagnosticErrorListener(true))
	p.BuildParseTrees = true
	tree := p.Expr()
  listener := NewTreeCalcListener()
	antlr.ParseTreeWalkerDefault.Walk(listener, tree)
  fmt.Println(listener.Values[tree])
  for _, values := range listener.Values { // 遍历查看获取的中间变量
    fmt.Println("value:", values)
  }
}

// 展示如何解析 string, 这个很常用
func StringParser() {
  fmt.Println("***************STRING PARSER***************")
  expr := "1+2+3"
  strInput := antlr.NewInputStream(expr) // 上面
	lexer := parser.NewCalcLexer(strInput)
	stream := antlr.NewCommonTokenStream(lexer,0)
	p := parser.NewCalcParser(stream)
	p.AddErrorListener(antlr.NewDiagnosticErrorListener(true))
	p.BuildParseTrees = true
	tree := p.Expr()
  listener := NewTreeCalcListener()
	antlr.ParseTreeWalkerDefault.Walk(listener, tree)
  fmt.Println(listener.Values[tree])
  for _, values := range listener.Values { // 遍历查看获取的中间变量
    fmt.Println("value:", values)
  }
}

func main() {
  // FileParser() // 从文件中读取解析
  StringParser() // 从字符串中读取解析
}
