/**
 * lemon parser definition.
 */

%right ASSIGN .
%left AND .
%left OR .
%right NOT .
%left LESS LESS_OR_EQUAL GREATER GREATER_OR_EQUAL EQUAL NOT_EQUAL .
%left PLUS MINUS .
%right UMINUS .
%left TIMES DIVIDE MOD .

%token_prefix ET_EXPR_
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
#include "state.h"
#include <assert.h>
#define UNUSED_ARG(x) (void) (x)
}
%include "namespace grammar { namespace expressions {"
%code "}}"
%token_destructor { UNUSED_ARG(state); UNUSED_ARG(yypminor); }
%stack_overflow {UNUSED_ARG(yypMinor); }

expression ::= expr(ROOT) . { state->root = ROOT; }

identifier(V) ::= IDENTIFIER(I) .
{
    V = create_identifier(I, state->vpool);
    add_to_temp_set(V, state->temp_set);
}

identifier(V) ::= identifier(I) DOT IDENTIFIER(F) .
{
    V = create_access_operator(I, F);
    remove_from_temp_set(I, state->temp_set);
    add_to_temp_set(V, state->temp_set);
}

term(V) ::= identifier(I) .  { V = I; }

term(T) ::= INTEGER(I) .
{
    T = create_integer(I);
    add_to_temp_set(T, state->temp_set);
}

term(T) ::= FLOAT(F) .
{
    T = create_float(F);
    add_to_temp_set(T, state->temp_set);
}

term(T) ::= BOOLEAN(B) .
{
    T = create_boolean(B);
    add_to_temp_set(T, state->temp_set);
}

term(T) ::= STRING(S) .
{
    T = create_string(S);
    add_to_temp_set(T, state->temp_set);
}

expr(E) ::= term(T) . { E = T; }

expr(R) ::= expr(A) PLUS expr(B) .
{
    R = create_add_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) MINUS expr(B) .
{
    R = create_sub_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) DIVIDE expr(B) .
{
    R = create_div_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) MOD expr(B) . 
{
    R = create_mod_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) TIMES expr(B) . 
{
    R = create_times_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= LEFT_P expr(E) RIGHT_P . 
{
    R = E;
}

expr(R) ::= MINUS expr(E) . [UMINUS] 
{
    R = create_minus_operator(E);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(E, state->temp_set);
}

expr(R) ::= NOT expr(E) .
{
    R = create_not_operator(E);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(E, state->temp_set);
}

expr(R) ::= expr(A) AND expr(B) .
{
    R = create_and_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) OR expr(B) .
{
    R = create_or_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) LESS expr(B) . 
{
    R = create_less_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) LESS_OR_EQUAL expr(B) . 
{
    R = create_less_or_equal_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) GREATER expr(B) . 
{
    R = create_greater_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) GREATER_OR_EQUAL expr(B) . 
{
    R = create_greater_or_equal_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) EQUAL expr(B) . 
{
    R = create_equal_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= expr(A) NOT_EQUAL expr(B) . 
{
    R = create_not_equal_operator(A, B);
    add_to_temp_set(R, state->temp_set);
    remove_from_temp_set(A, state->temp_set);
    remove_from_temp_set(B, state->temp_set);
}

expr(R) ::= identifier ASSIGN expr(B) .
{
    R = B; // fake assignment. Just pass through right part
}
