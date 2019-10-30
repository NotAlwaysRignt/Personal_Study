// Code generated from CalcParser.g4 by ANTLR 4.7.1. DO NOT EDIT.

package parser // CalcParser

import (
	"fmt"
	"reflect"
	"strconv"

	"github.com/antlr/antlr4/runtime/Go/antlr"
)

// Suppress unused import errors
var _ = fmt.Printf
var _ = reflect.Copy
var _ = strconv.Itoa

var parserATN = []uint16{
	3, 24715, 42794, 33075, 47597, 16764, 15335, 30598, 22884, 3, 10, 30, 4,
	2, 9, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 5, 2, 11, 10, 2, 3, 2, 3,
	2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 7, 2, 25,
	10, 2, 12, 2, 14, 2, 28, 11, 2, 3, 2, 2, 3, 2, 3, 2, 2, 2, 2, 33, 2, 10,
	3, 2, 2, 2, 4, 5, 8, 2, 1, 2, 5, 6, 7, 7, 2, 2, 6, 7, 5, 2, 2, 2, 7, 8,
	7, 8, 2, 2, 8, 11, 3, 2, 2, 2, 9, 11, 7, 9, 2, 2, 10, 4, 3, 2, 2, 2, 10,
	9, 3, 2, 2, 2, 11, 26, 3, 2, 2, 2, 12, 13, 12, 8, 2, 2, 13, 14, 7, 3, 2,
	2, 14, 25, 5, 2, 2, 9, 15, 16, 12, 7, 2, 2, 16, 17, 7, 4, 2, 2, 17, 25,
	5, 2, 2, 8, 18, 19, 12, 6, 2, 2, 19, 20, 7, 5, 2, 2, 20, 25, 5, 2, 2, 7,
	21, 22, 12, 5, 2, 2, 22, 23, 7, 6, 2, 2, 23, 25, 5, 2, 2, 6, 24, 12, 3,
	2, 2, 2, 24, 15, 3, 2, 2, 2, 24, 18, 3, 2, 2, 2, 24, 21, 3, 2, 2, 2, 25,
	28, 3, 2, 2, 2, 26, 24, 3, 2, 2, 2, 26, 27, 3, 2, 2, 2, 27, 3, 3, 2, 2,
	2, 28, 26, 3, 2, 2, 2, 5, 10, 24, 26,
}
var deserializer = antlr.NewATNDeserializer(nil)
var deserializedATN = deserializer.DeserializeFromUInt16(parserATN)

var literalNames = []string{
	"", "'*'", "'/'", "'+'", "'-'", "'('", "')'",
}
var symbolicNames = []string{
	"", "MULT", "DIV", "ADD", "DEC", "OPENPAREN", "CLOSEPAREN", "INT", "WS",
}

var ruleNames = []string{
	"expr",
}
var decisionToDFA = make([]*antlr.DFA, len(deserializedATN.DecisionToState))

func init() {
	for index, ds := range deserializedATN.DecisionToState {
		decisionToDFA[index] = antlr.NewDFA(ds, index)
	}
}

type CalcParser struct {
	*antlr.BaseParser
}

func NewCalcParser(input antlr.TokenStream) *CalcParser {
	this := new(CalcParser)

	this.BaseParser = antlr.NewBaseParser(input)

	this.Interpreter = antlr.NewParserATNSimulator(this, deserializedATN, decisionToDFA, antlr.NewPredictionContextCache())
	this.RuleNames = ruleNames
	this.LiteralNames = literalNames
	this.SymbolicNames = symbolicNames
	this.GrammarFileName = "CalcParser.g4"

	return this
}

// CalcParser tokens.
const (
	CalcParserEOF        = antlr.TokenEOF
	CalcParserMULT       = 1
	CalcParserDIV        = 2
	CalcParserADD        = 3
	CalcParserDEC        = 4
	CalcParserOPENPAREN  = 5
	CalcParserCLOSEPAREN = 6
	CalcParserINT        = 7
	CalcParserWS         = 8
)

// CalcParserRULE_expr is the CalcParser rule.
const CalcParserRULE_expr = 0

// IExprContext is an interface to support dynamic dispatch.
type IExprContext interface {
	antlr.ParserRuleContext

	// GetParser returns the parser.
	GetParser() antlr.Parser

	// IsExprContext differentiates from other interfaces.
	IsExprContext()
}

type ExprContext struct {
	*antlr.BaseParserRuleContext
	parser antlr.Parser
}

func NewEmptyExprContext() *ExprContext {
	var p = new(ExprContext)
	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(nil, -1)
	p.RuleIndex = CalcParserRULE_expr
	return p
}

func (*ExprContext) IsExprContext() {}

func NewExprContext(parser antlr.Parser, parent antlr.ParserRuleContext, invokingState int) *ExprContext {
	var p = new(ExprContext)

	p.BaseParserRuleContext = antlr.NewBaseParserRuleContext(parent, invokingState)

	p.parser = parser
	p.RuleIndex = CalcParserRULE_expr

	return p
}

func (s *ExprContext) GetParser() antlr.Parser { return s.parser }

func (s *ExprContext) CopyFrom(ctx *ExprContext) {
	s.BaseParserRuleContext.CopyFrom(ctx.BaseParserRuleContext)
}

func (s *ExprContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *ExprContext) ToStringTree(ruleNames []string, recog antlr.Recognizer) string {
	return antlr.TreesStringTree(s, ruleNames, recog)
}

type DIVContext struct {
	*ExprContext
}

func NewDIVContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *DIVContext {
	var p = new(DIVContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *DIVContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *DIVContext) AllExpr() []IExprContext {
	var ts = s.GetTypedRuleContexts(reflect.TypeOf((*IExprContext)(nil)).Elem())
	var tst = make([]IExprContext, len(ts))

	for i, t := range ts {
		if t != nil {
			tst[i] = t.(IExprContext)
		}
	}

	return tst
}

func (s *DIVContext) Expr(i int) IExprContext {
	var t = s.GetTypedRuleContext(reflect.TypeOf((*IExprContext)(nil)).Elem(), i)

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *DIVContext) DIV() antlr.TerminalNode {
	return s.GetToken(CalcParserDIV, 0)
}

func (s *DIVContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterDIV(s)
	}
}

func (s *DIVContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitDIV(s)
	}
}

type ADDContext struct {
	*ExprContext
}

func NewADDContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *ADDContext {
	var p = new(ADDContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *ADDContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *ADDContext) AllExpr() []IExprContext {
	var ts = s.GetTypedRuleContexts(reflect.TypeOf((*IExprContext)(nil)).Elem())
	var tst = make([]IExprContext, len(ts))

	for i, t := range ts {
		if t != nil {
			tst[i] = t.(IExprContext)
		}
	}

	return tst
}

func (s *ADDContext) Expr(i int) IExprContext {
	var t = s.GetTypedRuleContext(reflect.TypeOf((*IExprContext)(nil)).Elem(), i)

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *ADDContext) ADD() antlr.TerminalNode {
	return s.GetToken(CalcParserADD, 0)
}

func (s *ADDContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterADD(s)
	}
}

func (s *ADDContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitADD(s)
	}
}

type DECContext struct {
	*ExprContext
}

func NewDECContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *DECContext {
	var p = new(DECContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *DECContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *DECContext) AllExpr() []IExprContext {
	var ts = s.GetTypedRuleContexts(reflect.TypeOf((*IExprContext)(nil)).Elem())
	var tst = make([]IExprContext, len(ts))

	for i, t := range ts {
		if t != nil {
			tst[i] = t.(IExprContext)
		}
	}

	return tst
}

func (s *DECContext) Expr(i int) IExprContext {
	var t = s.GetTypedRuleContext(reflect.TypeOf((*IExprContext)(nil)).Elem(), i)

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *DECContext) DEC() antlr.TerminalNode {
	return s.GetToken(CalcParserDEC, 0)
}

func (s *DECContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterDEC(s)
	}
}

func (s *DECContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitDEC(s)
	}
}

type BRACKETContext struct {
	*ExprContext
}

func NewBRACKETContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *BRACKETContext {
	var p = new(BRACKETContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *BRACKETContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *BRACKETContext) OPENPAREN() antlr.TerminalNode {
	return s.GetToken(CalcParserOPENPAREN, 0)
}

func (s *BRACKETContext) Expr() IExprContext {
	var t = s.GetTypedRuleContext(reflect.TypeOf((*IExprContext)(nil)).Elem(), 0)

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *BRACKETContext) CLOSEPAREN() antlr.TerminalNode {
	return s.GetToken(CalcParserCLOSEPAREN, 0)
}

func (s *BRACKETContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterBRACKET(s)
	}
}

func (s *BRACKETContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitBRACKET(s)
	}
}

type MULTContext struct {
	*ExprContext
}

func NewMULTContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *MULTContext {
	var p = new(MULTContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *MULTContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *MULTContext) AllExpr() []IExprContext {
	var ts = s.GetTypedRuleContexts(reflect.TypeOf((*IExprContext)(nil)).Elem())
	var tst = make([]IExprContext, len(ts))

	for i, t := range ts {
		if t != nil {
			tst[i] = t.(IExprContext)
		}
	}

	return tst
}

func (s *MULTContext) Expr(i int) IExprContext {
	var t = s.GetTypedRuleContext(reflect.TypeOf((*IExprContext)(nil)).Elem(), i)

	if t == nil {
		return nil
	}

	return t.(IExprContext)
}

func (s *MULTContext) MULT() antlr.TerminalNode {
	return s.GetToken(CalcParserMULT, 0)
}

func (s *MULTContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterMULT(s)
	}
}

func (s *MULTContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitMULT(s)
	}
}

type INTContext struct {
	*ExprContext
}

func NewINTContext(parser antlr.Parser, ctx antlr.ParserRuleContext) *INTContext {
	var p = new(INTContext)

	p.ExprContext = NewEmptyExprContext()
	p.parser = parser
	p.CopyFrom(ctx.(*ExprContext))

	return p
}

func (s *INTContext) GetRuleContext() antlr.RuleContext {
	return s
}

func (s *INTContext) INT() antlr.TerminalNode {
	return s.GetToken(CalcParserINT, 0)
}

func (s *INTContext) EnterRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.EnterINT(s)
	}
}

func (s *INTContext) ExitRule(listener antlr.ParseTreeListener) {
	if listenerT, ok := listener.(CalcParserListener); ok {
		listenerT.ExitINT(s)
	}
}

func (p *CalcParser) Expr() (localctx IExprContext) {
	return p.expr(0)
}

func (p *CalcParser) expr(_p int) (localctx IExprContext) {
	var _parentctx antlr.ParserRuleContext = p.GetParserRuleContext()
	_parentState := p.GetState()
	localctx = NewExprContext(p, p.GetParserRuleContext(), _parentState)
	var _prevctx IExprContext = localctx
	var _ antlr.ParserRuleContext = _prevctx // TODO: To prevent unused variable warning.
	_startState := 0
	p.EnterRecursionRule(localctx, 0, CalcParserRULE_expr, _p)

	defer func() {
		p.UnrollRecursionContexts(_parentctx)
	}()

	defer func() {
		if err := recover(); err != nil {
			if v, ok := err.(antlr.RecognitionException); ok {
				localctx.SetException(v)
				p.GetErrorHandler().ReportError(p, v)
				p.GetErrorHandler().Recover(p, v)
			} else {
				panic(err)
			}
		}
	}()

	var _alt int

	p.EnterOuterAlt(localctx, 1)
	p.SetState(8)
	p.GetErrorHandler().Sync(p)

	switch p.GetTokenStream().LA(1) {
	case CalcParserOPENPAREN:
		localctx = NewBRACKETContext(p, localctx)
		p.SetParserRuleContext(localctx)
		_prevctx = localctx

		{
			p.SetState(3)
			p.Match(CalcParserOPENPAREN)
		}
		{
			p.SetState(4)
			p.expr(0)
		}
		{
			p.SetState(5)
			p.Match(CalcParserCLOSEPAREN)
		}

	case CalcParserINT:
		localctx = NewINTContext(p, localctx)
		p.SetParserRuleContext(localctx)
		_prevctx = localctx
		{
			p.SetState(7)
			p.Match(CalcParserINT)
		}

	default:
		panic(antlr.NewNoViableAltException(p, nil, nil, nil, nil, nil))
	}
	p.GetParserRuleContext().SetStop(p.GetTokenStream().LT(-1))
	p.SetState(24)
	p.GetErrorHandler().Sync(p)
	_alt = p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 2, p.GetParserRuleContext())

	for _alt != 2 && _alt != antlr.ATNInvalidAltNumber {
		if _alt == 1 {
			if p.GetParseListeners() != nil {
				p.TriggerExitRuleEvent()
			}
			_prevctx = localctx
			p.SetState(22)
			p.GetErrorHandler().Sync(p)
			switch p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 1, p.GetParserRuleContext()) {
			case 1:
				localctx = NewMULTContext(p, NewExprContext(p, _parentctx, _parentState))
				p.PushNewRecursionContext(localctx, _startState, CalcParserRULE_expr)
				p.SetState(10)

				if !(p.Precpred(p.GetParserRuleContext(), 6)) {
					panic(antlr.NewFailedPredicateException(p, "p.Precpred(p.GetParserRuleContext(), 6)", ""))
				}
				{
					p.SetState(11)
					p.Match(CalcParserMULT)
				}
				{
					p.SetState(12)
					p.expr(7)
				}

			case 2:
				localctx = NewDIVContext(p, NewExprContext(p, _parentctx, _parentState))
				p.PushNewRecursionContext(localctx, _startState, CalcParserRULE_expr)
				p.SetState(13)

				if !(p.Precpred(p.GetParserRuleContext(), 5)) {
					panic(antlr.NewFailedPredicateException(p, "p.Precpred(p.GetParserRuleContext(), 5)", ""))
				}
				{
					p.SetState(14)
					p.Match(CalcParserDIV)
				}
				{
					p.SetState(15)
					p.expr(6)
				}

			case 3:
				localctx = NewADDContext(p, NewExprContext(p, _parentctx, _parentState))
				p.PushNewRecursionContext(localctx, _startState, CalcParserRULE_expr)
				p.SetState(16)

				if !(p.Precpred(p.GetParserRuleContext(), 4)) {
					panic(antlr.NewFailedPredicateException(p, "p.Precpred(p.GetParserRuleContext(), 4)", ""))
				}
				{
					p.SetState(17)
					p.Match(CalcParserADD)
				}
				{
					p.SetState(18)
					p.expr(5)
				}

			case 4:
				localctx = NewDECContext(p, NewExprContext(p, _parentctx, _parentState))
				p.PushNewRecursionContext(localctx, _startState, CalcParserRULE_expr)
				p.SetState(19)

				if !(p.Precpred(p.GetParserRuleContext(), 3)) {
					panic(antlr.NewFailedPredicateException(p, "p.Precpred(p.GetParserRuleContext(), 3)", ""))
				}
				{
					p.SetState(20)
					p.Match(CalcParserDEC)
				}
				{
					p.SetState(21)
					p.expr(4)
				}

			}

		}
		p.SetState(26)
		p.GetErrorHandler().Sync(p)
		_alt = p.GetInterpreter().AdaptivePredict(p.GetTokenStream(), 2, p.GetParserRuleContext())
	}

	return localctx
}

func (p *CalcParser) Sempred(localctx antlr.RuleContext, ruleIndex, predIndex int) bool {
	switch ruleIndex {
	case 0:
		var t *ExprContext = nil
		if localctx != nil {
			t = localctx.(*ExprContext)
		}
		return p.Expr_Sempred(t, predIndex)

	default:
		panic("No predicate with index: " + fmt.Sprint(ruleIndex))
	}
}

func (p *CalcParser) Expr_Sempred(localctx antlr.RuleContext, predIndex int) bool {
	switch predIndex {
	case 0:
		return p.Precpred(p.GetParserRuleContext(), 6)

	case 1:
		return p.Precpred(p.GetParserRuleContext(), 5)

	case 2:
		return p.Precpred(p.GetParserRuleContext(), 4)

	case 3:
		return p.Precpred(p.GetParserRuleContext(), 3)

	default:
		panic("No predicate with index: " + fmt.Sprint(predIndex))
	}
}
