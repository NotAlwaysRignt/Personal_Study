lexer grammar CalcLexer;

MULT: '*';
DIV: '/';
ADD: '+';
DEC: '-';

OPENPAREN: '(';
CLOSEPAREN: ')';

INT: [0-9]+;
WS: [ \t\r\n]+ -> skip ;
