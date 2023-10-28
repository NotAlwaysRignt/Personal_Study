// Code generated from CalcParser.g4 by ANTLR 4.7.1. DO NOT EDIT.

package parser // CalcParser

import "github.com/antlr/antlr4/runtime/Go/antlr"

// CalcParserListener is a complete listener for a parse tree produced by CalcParser.
type CalcParserListener interface {
	antlr.ParseTreeListener

	// EnterDIV is called when entering the DIV production.
	EnterDIV(c *DIVContext)

	// EnterADD is called when entering the ADD production.
	EnterADD(c *ADDContext)

	// EnterDEC is called when entering the DEC production.
	EnterDEC(c *DECContext)

	// EnterBRACKET is called when entering the BRACKET production.
	EnterBRACKET(c *BRACKETContext)

	// EnterMULT is called when entering the MULT production.
	EnterMULT(c *MULTContext)

	// EnterINT is called when entering the INT production.
	EnterINT(c *INTContext)

	// ExitDIV is called when exiting the DIV production.
	ExitDIV(c *DIVContext)

	// ExitADD is called when exiting the ADD production.
	ExitADD(c *ADDContext)

	// ExitDEC is called when exiting the DEC production.
	ExitDEC(c *DECContext)

	// ExitBRACKET is called when exiting the BRACKET production.
	ExitBRACKET(c *BRACKETContext)

	// ExitMULT is called when exiting the MULT production.
	ExitMULT(c *MULTContext)

	// ExitINT is called when exiting the INT production.
	ExitINT(c *INTContext)
}
