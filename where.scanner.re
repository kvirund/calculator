/**
 * re2c scanner definition
 * vim: set ts=4 sw=4 tw=0 et syntax=cpp:
 */

#include "where.scanner.h"

#include <string>
#include <algorithm>

namespace scanner
{
namespace where
{

/*!types:re2c*/

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
re2c:indent:top = 2;

SPACE = [ \t\r\n]* ;
NULL = 'NULL' ;
QUOTED_IDENTIFIER = '"' [^\x00"]+ '"' ;
IDENTIFIER = [a-zA-Z_][a-zA-Z0-9_]* ;
DOT = '.' ;
LEFT_P = '(' ;
LESS = '<' ;
GREATER = '>' ;
LESS_OR_EQUAL = '<=' ;
GREATER_OR_EQUAL = '>=' ;
EQUAL = '==' | '=' ;
NOT_EQUAL = '!=' | '<>' ;
LIKE = 'like' ;
IS = 'is' ;
IS_NOT = 'is' SPACE+ 'not' ;
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
STRING = '\'' ( [^\x00'\\] | '\\\\' | '\\\'' )* '\'' ;
RIGHT_P = ')' ;
END = "\x00" ;
UNDOREDO = 'undo' | 'redo' ;

SPACE            { state.move(cursor - p_start); goto beginning; }
BOOLEAN          { state.move(cursor - p_start); return CToken(ET_WHERE_BOOLEAN, expression.substr(start, cursor - p_start)); }
INTEGER          { state.move(cursor - p_start); return CToken(ET_WHERE_INTEGER, expression.substr(start, cursor - p_start)); }
FLOAT            { state.move(cursor - p_start); return CToken(ET_WHERE_FLOAT, expression.substr(start, cursor - p_start)); }
STRING           { state.move(cursor - p_start); return CToken(ET_WHERE_STRING, expression.substr(1 + start, cursor - p_start - 2), true); }

LESS_OR_EQUAL    { state.move(cursor - p_start); return CToken(ET_WHERE_LESS_OR_EQUAL, expression.substr(start, cursor - p_start)); }
GREATER_OR_EQUAL { state.move(cursor - p_start); return CToken(ET_WHERE_GREATER_OR_EQUAL, expression.substr(start, cursor - p_start)); }
EQUAL            { state.move(cursor - p_start); return CToken(ET_WHERE_EQUAL, expression.substr(start, cursor - p_start)); }
NOT_EQUAL        { state.move(cursor - p_start); return CToken(ET_WHERE_NOT_EQUAL, expression.substr(start, cursor - p_start)); }
LESS             { state.move(cursor - p_start); return CToken(ET_WHERE_LESS, expression.substr(start, cursor - p_start)); }
GREATER          { state.move(cursor - p_start); return CToken(ET_WHERE_GREATER, expression.substr(start, cursor - p_start)); }

LIKE             { state.move(cursor - p_start); return CToken(ET_WHERE_LIKE, expression.substr(start, cursor - p_start)); }
IS               { state.move(cursor - p_start); return CToken(ET_WHERE_IS, expression.substr(start, cursor - p_start)); }
IS_NOT           { state.move(cursor - p_start); return CToken(ET_WHERE_IS_NOT, expression.substr(start, cursor - p_start)); }

AND              { state.move(cursor - p_start); return CToken(ET_WHERE_AND, expression.substr(start, cursor - p_start)); }
OR               { state.move(cursor - p_start); return CToken(ET_WHERE_OR, expression.substr(start, cursor - p_start)); }
NOT              { state.move(cursor - p_start); return CToken(ET_WHERE_NOT, expression.substr(start, cursor - p_start)); }

PLUS             { state.move(cursor - p_start); return CToken(ET_WHERE_PLUS, expression.substr(start, cursor - p_start)); }
MINUS            { state.move(cursor - p_start); return CToken(ET_WHERE_MINUS, expression.substr(start, cursor - p_start)); }

DIVIDE           { state.move(cursor - p_start); return CToken(ET_WHERE_DIVIDE, expression.substr(start, cursor - p_start)); }
MOD              { state.move(cursor - p_start); return CToken(ET_WHERE_MOD, expression.substr(start, cursor - p_start)); }
TIMES            { state.move(cursor - p_start); return CToken(ET_WHERE_TIMES, expression.substr(start, cursor - p_start)); }

NULL             { state.move(cursor - p_start); return CToken(ET_WHERE_NULL, expression.substr(start, cursor - p_start)); }

UNDOREDO
{
    state.move(cursor - p_start);
    std::string var = expression.substr(start, cursor - p_start);
    std::transform(var.begin(), var.end(), var.begin(), ::toupper);
    return CToken(ET_WHERE_FIELD, var);
}

QUOTED_IDENTIFIER
{
    state.move(cursor - p_start);
    return CToken(ET_WHERE_IDENTIFIER, expression.substr(1 + start, cursor - p_start - 2));
}

IDENTIFIER
{
    state.move(cursor - p_start);
    std::string var = expression.substr(start, cursor - p_start);
    std::transform(var.begin(), var.end(), var.begin(), ::toupper);
    return CToken(ET_WHERE_IDENTIFIER, var);
}

DOT
{
    state.move(cursor - p_start);
    return CToken(ET_WHERE_DOT, expression.substr(start, cursor - p_start));
}

LEFT_P           { state.move(cursor - p_start); return CToken(ET_WHERE_LEFT_P, expression.substr(start, cursor - p_start)); }
RIGHT_P          { state.move(cursor - p_start); return CToken(ET_WHERE_RIGHT_P, expression.substr(start, cursor - p_start)); }

END              { return CToken(CToken::ET_EOF, expression.substr(start, cursor - p_start)); }
[^]              { return CToken(CToken::ET_ERROR, expression.substr(start, cursor - p_start)); }

*/
    }
}

}
}
