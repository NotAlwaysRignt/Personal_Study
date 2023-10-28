package main

import (
	"github.com/antlr/antlr4/runtime/Go/antlr"
	"json/parser"
	"os"
	"fmt"
)

type TreeShapeListener struct {
	*parser.BaseJSONListener
}

func NewTreeShapeListener() *TreeShapeListener {
	return new(TreeShapeListener)
}

func (this *TreeShapeListener) EnterEveryRule(ctx antlr.ParserRuleContext) {
	fmt.Println(ctx.GetText())
}

func main() {
	input, err := antlr.NewFileStream(os.Args[1])
  if(err != nil) {
    fmt.Println(err.Error())
    return
  }
	lexer := parser.NewJSONLexer(input)
	stream := antlr.NewCommonTokenStream(lexer,0)
	p := parser.NewJSONParser(stream)
	p.AddErrorListener(antlr.NewDiagnosticErrorListener(true))
	p.BuildParseTrees = true
	tree := p.Json()
	antlr.ParseTreeWalkerDefault.Walk(NewTreeShapeListener(), tree)
}
