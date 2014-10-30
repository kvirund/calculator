%left AND .
%left OR .
%left LESS LESS_OR_EQUAL GREATER GREATER_OR_EQUAL EQUAL NOT_EQUAL .
%left PLUS MINUS .
%left TIMES DIVIDE MOD .
%right NOT .
%left SPACE .

%token_prefix ET_
%token_type { void* }
%extra_argument { struct SState* state }
%parse_failure {
    state->syntax_error = 1;
}
%syntax_error {
    state->syntax_error = 1;
}
%include {
#include <assert.h>
#include "state.h"
}

expression ::= expr(ROOT) . { set_root(state->tree, ROOT); }

term(T) ::= IDENTIFIER(I) .
    {
        T = create_identifier(I);
    }
term(T) ::= INTEGER(I) .
    {
        T = create_integer(I);
    }
term(T) ::= FLOAT(F) .
    {
        T = create_float(F);
    }
term(T) ::= BOOLEAN(B) .
    {
        T = create_boolean(B);
    }
term(T) ::= STRING(S) .
    {
        T = create_string(S);
    }

expr(E) ::= term(T) . { E = T; }

expr(R) ::= SPACE expr(E) . { R = E; }
expr(R) ::= expr(E) SPACE . {R = E; }
expr(R) ::= expr(A) PLUS expr(B) . { R = create_add_operator(A, B); }
expr(R) ::= expr(A) MINUS expr(B) . { R = create_sub_operator(A, B); }
expr(R) ::= expr(A) DIVIDE expr(B) . { R = create_div_operator(A, B); }
expr(R) ::= expr(A) MOD expr(B) . { R = create_mod_operator(A, B); }
expr(R) ::= expr(A) TIMES expr(B) . { R = create_times_operator(A, B); }
expr(R) ::= LEFT_P expr(E) RIGHT_P . { R = E; }
expr(R) ::= MINUS expr(E) . [NOT] { R = create_minus_operator(E); }
expr(R) ::= expr(A) LESS expr(B) . { R = create_less_operator(A, B); }
expr(R) ::= expr(A) LESS_OR_EQUAL expr(B) . { R = create_less_or_equal_operator(A, B); }
expr(R) ::= expr(A) GREATER expr(B) . { R = create_greater_operator(A, B); }
expr(R) ::= expr(A) GREATER_OR_EQUAL expr(B) . { R = create_greater_or_equal_operator(A, B); }
expr(R) ::= expr(A) EQUAL expr(B) . { R = create_equal_operator(A, B); }
expr(R) ::= expr(A) NOT_EQUAL expr(B) . { R = create_not_equal_operator(A, B); }
