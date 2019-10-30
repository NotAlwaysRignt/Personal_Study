// Code generated from CalcParser.g4 by ANTLR 4.7.1. DO NOT EDIT.

package parser // CalcParser

import "github.com/antlr/antlr4/runtime/Go/antlr"

// BaseCalcParserListener is a complete listener for a parse tree produced by CalcParser.
type BaseCalcParserListener struct{}

var _ CalcParserListener = &BaseCalcParserListener{}

// VisitTerminal is called when a terminal node is visited.
func (s *BaseCalcParserListener) VisitTerminal(node antlr.TerminalNode) {}

// VisitErrorNode is called when an error node is visited.
func (s *BaseCalcParserListener) VisitErrorNode(node antlr.ErrorNode) {}

// EnterEveryRule is called when any rule is entered.
func (s *BaseCalcParserListener) EnterEveryRule(ctx antlr.ParserRuleContext) {}

// ExitEveryRule is called when any rule is exited.
func (s *BaseCalcParserListener) ExitEveryRule(ctx antlr.ParserRuleContext) {}

// EnterDIV is called when production DIV is entered.
func (s *BaseCalcParserListener) EnterDIV(ctx *DIVContext) {}

// ExitDIV is called when production DIV is exited.
func (s *BaseCalcParserListener) ExitDIV(ctx *DIVContext) {}

// EnterADD is called when production ADD is entered.
func (s *BaseCalcParserListener) EnterADD(ctx *ADDContext) {}

// ExitADD is called when production ADD is exited.
func (s *BaseCalcParserListener) ExitADD(ctx *ADDContext) {}

// EnterDEC is called when production DEC is entered.
func (s *BaseCalcParserListener) EnterDEC(ctx *DECContext) {}

// ExitDEC is called when production DEC is exited.
func (s *BaseCalcParserListener) ExitDEC(ctx *DECContext) {}

// EnterBRACKET is called when production BRACKET is entered.
func (s *BaseCalcParserListener) EnterBRACKET(ctx *BRACKETContext) {}

// ExitBRACKET is called when production BRACKET is exited.
func (s *BaseCalcParserListener) ExitBRACKET(ctx *BRACKETContext) {}

// EnterMULT is called when production MULT is entered.
func (s *BaseCalcParserListener) EnterMULT(ctx *MULTContext) {}

// ExitMULT is called when production MULT is exited.
func (s *BaseCalcParserListener) ExitMULT(ctx *MULTContext) {}

// EnterINT is called when production INT is entered.
func (s *BaseCalcParserListener) EnterINT(ctx *INTContext) {}

// ExitINT is called when production INT is exited.
func (s *BaseCalcParserListener) ExitINT(ctx *INTContext) {}
