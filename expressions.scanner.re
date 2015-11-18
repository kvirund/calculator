/**
 * re2c scanner definition
 * vim: set ts=4 sw=4 tw=0 et syntax=cpp:
 */

#include "expressions.scanner.h"

namespace scanner
{
namespace expressions
{

CToken CScanner::operator()(const std::string& expression, CState& state) const
{
beginning:
    {
        std::string::const_pointer marker;
        std::string::const_pointer cursor = expression.c_str() + state.offset();
        std::string::const_pointer p_start = expression.c_str() + state.offset();
        size_t start = state.offset();

/*!re2c

re2c:define:YYCTYPE = "unsigned char";
re2c:define:YYCURSOR = cursor;
re2c:define:YYMARKER = marker;
re2c:yyfill:enable = 0;
re2c:yych:conversion = 1;
re2c:indent:string = "    ";
re2c:indent:top = 1;

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
AND = '&&' | 'and' ;
OR = '||' | 'or' ;
NOT = '!' | 'not' ;
BOOLEAN = 'true' | 'false' ;
INTEGER = [0-9]+ ;
FLOAT = [0-9]+ ('.' [0-9]*)? ('e'[+-]?[0-9]+)? ;
STRING = ('"' ( [^\x00"\\] | '\\\\' | '\\"' )* '"') | ('\'' ( [^\x00'\\] | '\\\\' | '\\\'' )* '\'') ;
RIGHT_P = ')' ;
SPACE = [ \t\r\n]* ;
END = "\x00" ;

ASSIGN = '=' ;
IDENTIFIER = [a-zA-Z_][a-zA-Z0-9_]* ;
DOT = '.' ;

SPACE            { state.move(cursor - p_start); goto beginning; }
BOOLEAN          { state.move(cursor - p_start); return CToken(ET_EXPR_BOOLEAN, expression.substr(start, cursor - p_start)); }
INTEGER          { state.move(cursor - p_start); return CToken(ET_EXPR_INTEGER, expression.substr(start, cursor - p_start)); }
FLOAT            { state.move(cursor - p_start); return CToken(ET_EXPR_FLOAT, expression.substr(start, cursor - p_start)); }
STRING           { state.move(cursor - p_start); return CToken(ET_EXPR_STRING, expression.substr(1 + start, cursor - p_start - 2), true); }

ASSIGN           { state.move(cursor - p_start); return CToken(ET_EXPR_ASSIGN, expression.substr(start, cursor - p_start)); }

LESS             { state.move(cursor - p_start); return CToken(ET_EXPR_LESS, expression.substr(start, cursor - p_start)); }
GREATER          { state.move(cursor - p_start); return CToken(ET_EXPR_GREATER, expression.substr(start, cursor - p_start)); }
LESS_OR_EQUAL    { state.move(cursor - p_start); return CToken(ET_EXPR_LESS_OR_EQUAL, expression.substr(start, cursor - p_start)); }
GREATER_OR_EQUAL { state.move(cursor - p_start); return CToken(ET_EXPR_GREATER_OR_EQUAL, expression.substr(start, cursor - p_start)); }
EQUAL            { state.move(cursor - p_start); return CToken(ET_EXPR_EQUAL, expression.substr(start, cursor - p_start)); }
NOT_EQUAL        { state.move(cursor - p_start); return CToken(ET_EXPR_NOT_EQUAL, expression.substr(start, cursor - p_start)); }

AND              { state.move(cursor - p_start); return CToken(ET_EXPR_AND, expression.substr(start, cursor - p_start)); }
OR               { state.move(cursor - p_start); return CToken(ET_EXPR_OR, expression.substr(start, cursor - p_start)); }
NOT              { state.move(cursor - p_start); return CToken(ET_EXPR_NOT, expression.substr(start, cursor - p_start)); }

IDENTIFIER       { state.move(cursor - p_start); return CToken(ET_EXPR_IDENTIFIER, expression.substr(start, cursor - p_start)); }
DOT              { state.move(cursor - p_start); return CToken(ET_EXPR_DOT, expression.substr(start, cursor - p_start)); }

PLUS             { state.move(cursor - p_start); return CToken(ET_EXPR_PLUS, expression.substr(start, cursor - p_start)); }
MINUS            { state.move(cursor - p_start); return CToken(ET_EXPR_MINUS, expression.substr(start, cursor - p_start)); }

DIVIDE           { state.move(cursor - p_start); return CToken(ET_EXPR_DIVIDE, expression.substr(start, cursor - p_start)); }
MOD              { state.move(cursor - p_start); return CToken(ET_EXPR_MOD, expression.substr(start, cursor - p_start)); }
TIMES            { state.move(cursor - p_start); return CToken(ET_EXPR_TIMES, expression.substr(start, cursor - p_start)); }

LEFT_P           { state.move(cursor - p_start); return CToken(ET_EXPR_LEFT_P, expression.substr(start, cursor - p_start)); }
RIGHT_P          { state.move(cursor - p_start); return CToken(ET_EXPR_RIGHT_P, expression.substr(start, cursor - p_start)); }

END              { state.move(cursor - p_start); return CToken(CToken::ET_EOF, expression.substr(start, cursor - p_start)); }
[^]              { state.move(cursor - p_start); return CToken(CToken::ET_ERROR, expression.substr(start, cursor - p_start)); }

*/
    }
}

}
}
