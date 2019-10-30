// 以生成 golang 目标文件为例, 需要调用 antlr4 -Dlanguage=Go CalcParser.g4 CalcLexer.g4 而不是只用 antlr4 -Dlanguage=Go CalcParser.g4 否则会失败
parser grammar CalcParser; // 注意是 grammar 而不是 grammer
options {
  tokenVocab=CalcLexer;
}

// 在parser grammar 文件里, 不可以直接写入token
// 比如用 '(' expr ')' 会报错, 必须把'('和')'的定义写在 lexer 文件中, 然后以记号的形式引入
expr
  : expr MULT expr  # MULT
  | expr DIV expr   # DIV
  | expr ADD expr   # ADD
  | expr DEC expr   # DEC
  | OPENPAREN expr CLOSEPAREN    # BRACKET
  | INT             # INT
  // | (expr [\r\n]+)* # NEWLINE
  ;
