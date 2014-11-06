#include "scanner.h"

namespace scanner
{

CToken scan(const std::string& expression, size_t& offset)
{
    std::string::const_pointer marker;
    std::string::const_pointer cursor = expression.c_str() + offset;
    std::string::const_pointer p_start = expression.c_str() + offset;
    size_t start = offset;

/*!re2c

re2c:define:YYCTYPE = "unsigned char";
re2c:define:YYCURSOR = cursor;
re2c:define:YYMARKER = marker;
re2c:yyfill:enable = 0;
re2c:yych:conversion = 1;
re2c:indent:string = "    ";
re2c:indent:top = 1;

IDENTIFIER = [a-zA-Z_][a-zA-Z0-9_]* ;
LEFT_P = '(' ;
LESS = '<' ;
GREATER = '>' ;
LESS_OR_EQUAL = '<=' ;
GREATER_OR_EQUAL = '>=' ;
EQUAL = '==' ;
NOT_EQUAL = '!=' ;
PLUS = '+' ;
MINUS = '-' ;
DIVIDE = '/' ;
MOD = '%' ;
TIMES = '*' ;
AND = 'and' ;
OR = 'or' ;
NOT = 'not' ;
BOOLEAN = 'true' | 'false' ;
INTEGER = [0-9]+ ;
FLOAT = [0-9]+ ('.' [0-9]*)? ('e'[+-]?[0-9]+)? ;
STRING = '"' ( [^"\\\\] | '\\\\' | '\\"' )* '"' ;
RIGHT_P = ')' ;
SPACE = [ \t]* ;
EOL = '\n' ;
END = "\x00" ;
ASSIGN = '=' \ [^=] ;

SPACE            { offset += cursor - p_start; return CToken(ET_SPACE, expression.substr(start, cursor - p_start)); }
BOOLEAN          { offset += cursor - p_start; return CToken(ET_BOOLEAN, expression.substr(start, cursor - p_start)); }
FLOAT            { offset += cursor - p_start; return CToken(ET_FLOAT, expression.substr(start, cursor - p_start)); }
INTEGER          { offset += cursor - p_start; return CToken(ET_INTEGER, expression.substr(start, cursor - p_start)); }
STRING           { offset += cursor - p_start; return CToken(ET_STRING, expression.substr(1 + start, cursor - p_start - 2)); }

LESS             { offset += cursor - p_start; return CToken(ET_LESS, expression.substr(start, cursor - p_start)); }
GREATER          { offset += cursor - p_start; return CToken(ET_GREATER, expression.substr(start, cursor - p_start)); }
LESS_OR_EQUAL    { offset += cursor - p_start; return CToken(ET_LESS_OR_EQUAL, expression.substr(start, cursor - p_start)); }
GREATER_OR_EQUAL { offset += cursor - p_start; return CToken(ET_GREATER_OR_EQUAL, expression.substr(start, cursor - p_start)); }
EQUAL            { offset += cursor - p_start; return CToken(ET_EQUAL, expression.substr(start, cursor - p_start)); }
NOT_EQUAL        { offset += cursor - p_start; return CToken(ET_NOT_EQUAL, expression.substr(start, cursor - p_start)); }

AND              { offset += cursor - p_start; return CToken(ET_AND, expression.substr(start, cursor - p_start)); }
OR               { offset += cursor - p_start; return CToken(ET_OR, expression.substr(start, cursor - p_start)); }
NOT              { offset += cursor - p_start; return CToken(ET_NOT, expression.substr(start, cursor - p_start)); }

PLUS             { offset += cursor - p_start; return CToken(ET_PLUS, expression.substr(start, cursor - p_start)); }
MINUS            { offset += cursor - p_start; return CToken(ET_MINUS, expression.substr(start, cursor - p_start)); }

DIVIDE           { offset += cursor - p_start; return CToken(ET_DIVIDE, expression.substr(start, cursor - p_start)); }
MOD              { offset += cursor - p_start; return CToken(ET_MOD, expression.substr(start, cursor - p_start)); }
TIMES            { offset += cursor - p_start; return CToken(ET_TIMES, expression.substr(start, cursor - p_start)); }

IDENTIFIER       { offset += cursor - p_start; return CToken(ET_IDENTIFIER, expression.substr(start, cursor - p_start)); }

LEFT_P           { offset += cursor - p_start; return CToken(ET_LEFT_P, expression.substr(start, cursor - p_start)); }
RIGHT_P          { offset += cursor - p_start; return CToken(ET_RIGHT_P, expression.substr(start, cursor - p_start)); }

ASSIGN           { offset += cursor - p_start; return CToken(ET_ASSIGN, expression.substr(start, cursor - p_start)); }

END              { offset += cursor - p_start; return CToken(ET_EOF, expression.substr(start, cursor - p_start)); }
[^]              { offset += cursor - p_start; return CToken(ET_ERROR, expression.substr(start, cursor - p_start)); }

*/
}
}
