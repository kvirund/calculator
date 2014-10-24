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
OPERATION = '<' | '>' | '<=' | '>=' | '==' | '!=' | '+' | '-' | '/' | '*' ;
BOOLEAN = 'true' | 'false' ;
INTEGER = [0-9]+ ;
FLOAT = [0-9]+ ('.' [0-9]*)? ('e'[+-][0-9]+)? ;
STRING = '"' ( [^"\\\\] | '\\\\' | '\\"' )* '"' ;
RIGHT_P = ')' ;
SPACE = [ \t]* ;
EOL = '\n' ;
END = "\x00" ;

SPACE       { offset += cursor - p_start; return CToken(ET_SPACE, expression.substr(start, cursor - p_start)); }
BOOLEAN     { offset += cursor - p_start; return CToken(ET_BOOLEAN, expression.substr(start, cursor - p_start)); }
FLOAT       { offset += cursor - p_start; return CToken(ET_FLOAT, expression.substr(start, cursor - p_start)); }
INTEGER     { offset += cursor - p_start; return CToken(ET_INTEGER, expression.substr(start, cursor - p_start)); }
STRING      { offset += cursor - p_start; return CToken(ET_STRING, expression.substr(start, cursor - p_start)); }
IDENTIFIER  { offset += cursor - p_start; return CToken(ET_IDENTIFIER, expression.substr(start, cursor - p_start)); }
OPERATION   { offset += cursor - p_start; return CToken(ET_OPERATION, expression.substr(start, cursor - p_start)); }
LEFT_P      { offset += cursor - p_start; return CToken(ET_LEFT_PARENTHESIS, expression.substr(start, cursor - p_start)); }
RIGHT_P     { offset += cursor - p_start; return CToken(ET_RIGHT_PARENTHESIS, expression.substr(start, cursor - p_start)); }
EOL         { offset += cursor - p_start; return CToken(ET_EOL, expression.substr(start, cursor - p_start)); }
END         { offset += cursor - p_start; return CToken(ET_END, expression.substr(start, cursor - p_start)); }
[^]         { offset += cursor - p_start; return CToken(ET_ERROR, expression.substr(start, cursor - p_start)); }

*/
}
}
