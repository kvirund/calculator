/**
 * re2c scanner definition for testing console
 * vim: set ts=4 sw=4 tw=0 et syntax=cpp:
 */
#include "console.scanner.h"
#include "console.grammar.h"

#include <string.h>

#include <algorithm>

namespace scanner
{
    namespace console
    {

/*!types:re2c*/

        CToken CScanner::operator()(const std::string& expression, CState& state) const
        {
beginning:
            {
                std::string::const_pointer marker;
                std::string::const_pointer cursor = expression.c_str() + state.offset();
                std::string::const_pointer limit = expression.c_str() + expression.length();
                std::string::const_pointer p_start = expression.c_str() + state.offset();
                size_t start = state.offset();

/*!re2c
re2c:define:YYCTYPE = "unsigned char";
re2c:define:YYCURSOR = cursor;
re2c:define:YYMARKER = marker;
re2c:define:YYGETCONDITION = "state.condition()";
re2c:define:YYGETCONDITION:naked = 1;
re2c:define:YYSETCONDITION = "state.condition(@@);";
re2c:define:YYSETCONDITION@cond = @@;
re2c:yyfill:enable = 0;
re2c:yych:conversion = 1;
re2c:indent:string = "    ";
re2c:indent:top = 4;

SPACE = [ \r\t\n]+ ;
SWITCH = '.where' | '.expr' | '.dump' ;
SET_COMMAND = '.set' [ \rt] ;
EXEC_COMMAND = '.exec' [ \rt] ;
QUOTED_IDENTIFIER = '"' [^\x00"]+ '"' ;
IDENTIFIER = [a-zA-Z_][a-zA-Z0-9_]* ;
DOT = '.' ;
ASSIGN = '=' ;
UNDOREDO = 'undo' | 'redo' ;
END = '\x00' ;

<> :=> normal

<normal,expression,set> SPACE { state.move(cursor - p_start); goto beginning; }

<normal> SWITCH         { state.move(cursor - p_start); return CToken(ET_CONSOLE_SWITCH, expression.substr(start, cursor - p_start)); }

<normal> SET_COMMAND => set
{
    state.move(cursor - p_start);
    return CToken(ET_CONSOLE_SET_COMMAND, expression.substr(start, cursor - p_start - 1));
}

<normal> EXEC_COMMAND => expression
{
    state.move(cursor - p_start);
    return CToken(ET_CONSOLE_EXEC_COMMAND, expression.substr(start, cursor - p_start - 1));
}

<set> UNDOREDO
{
    state.move(cursor - p_start);
    std::string var = expression.substr(start, cursor - p_start);
    std::transform(var.begin(), var.end(), var.begin(), ::toupper);
    return CToken(ET_CONSOLE_UNDOREDO, var);
}

<set> QUOTED_IDENTIFIER
{
    state.move(cursor - p_start);
    return CToken(ET_CONSOLE_IDENTIFIER, expression.substr(1 + start, cursor - p_start - 2));
}

<set> IDENTIFIER
{
    state.move(cursor - p_start);
    return CToken(ET_CONSOLE_IDENTIFIER, expression.substr(start, cursor - p_start));
}

<set> ASSIGN => expression
{
    state.move(cursor - p_start);
    return CToken(ET_CONSOLE_ASSIGN, expression.substr(start, cursor - p_start));
}

<set> DOT               { state.move(cursor - p_start); return CToken(ET_CONSOLE_DOT, expression.substr(start, cursor - p_start)); }

<normal,expression> [^\x00] { state.move(limit - p_start); return CToken(ET_CONSOLE_TAIL,  expression.substr(start, limit - p_start)); }
<*> END                 { return CToken(CToken::ET_EOF, expression.substr(start, cursor - p_start)); }
<*> [^]                 { return CToken(CToken::ET_ERROR, expression.substr(start, cursor - p_start)); }

*/
            }
        }
    }
}
