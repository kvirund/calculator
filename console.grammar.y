/**
 * lemon parser definition.
 */

%left DOT .

%token_prefix ET_CONSOLE_
%token_type { void* }
%extra_argument { SState* state }
%parse_failure {
    state->syntax_error = 1;
}
%syntax_error {
    UNUSED_ARG(yymajor);
    UNUSED_ARG(yyminor);
    state->syntax_error = 1;
}
%include {
#include "console.h"

#include "state.h"
#include "parser.h"

#include <assert.h>

#define UNUSED_ARG(x) (void) (x)
}
%include "namespace grammar { namespace console {"
%code "}}"

%token_destructor { UNUSED_ARG(state); UNUSED_ARG(yypminor); }
%stack_overflow {UNUSED_ARG(yypMinor); }

expression ::= expr .

identifier(V) ::= IDENTIFIER(I) .
{
    V = create_identifier(I, state->vpool);
    add_to_temp_set(V, state->temp_set);
}

identifier(V) ::= identifier(I) DOT IDENTIFIER(J) .
{
    V = create_access_operator(I, J);
    remove_from_temp_set(I, state->temp_set);
    add_to_temp_set(V, state->temp_set);
}

identifier(V) ::= identifier(I) DOT UNDOREDO(F) .
{
    V = create_access_operator(I, F);
    remove_from_temp_set(I, state->temp_set);
    add_to_temp_set(V, state->temp_set);
}

expr ::= SWITCH(CMD) .
{
    parser::CTreeNode* cmd = static_cast<parser::CTreeNode*>(CMD);
    static_cast<CConsole*>(state->root)->set_command(cmd->value().get_string());
}

expr ::= SET_COMMAND(CMD) identifier(I) ASSIGN TAIL(EXPR) .
{
    parser::CParserNode* cmd = static_cast<parser::CParserNode*>(CMD);
    parser::CParserNode* expr = static_cast<parser::CParserNode*>(EXPR);
    CConsole* console = static_cast<CConsole*>(state->root);
    console->set_command(cmd->value().get_string());        // set command
    console->set_expression(expr->value().get_string());    // set expression for that variable (will be parsed and calculated by some different parser)
    parser::CObjectTreeNode* var = static_cast<parser::CObjectTreeNode*>(I);
    console->set_variable(var);                             // set variable name we want assign to
}

expr ::= EXEC_COMMAND(CMD) TAIL(EXPR) .
{
    parser::CParserNode* cmd = static_cast<parser::CParserNode*>(CMD);
    parser::CParserNode* expr = static_cast<parser::CParserNode*>(EXPR);
    CConsole* console = static_cast<CConsole*>(state->root);
    console->set_command(cmd->value().get_string());        // set command
    console->set_expression(expr->value().get_string());    // set expression for that variable (will be parsed and calculated by some different parser)
}

expr ::= TAIL(EXPR) .
{
    parser::CParserNode* expr = static_cast<parser::CParserNode*>(EXPR);
    CConsole* console = static_cast<CConsole*>(state->root);
    console->set_command(".exec");                          // default command
    console->set_expression(expr->value().get_string());    // set expression for that variable (will be parsed and calculated by some different parser)
}
