%left AND .
%left OR .
%left LESS LESS_OR_EQUAL GREATER GREATER_OR_EQUAL EQUAL NOT_EQUAL .
%left PLUS MINUS .
%left TIMES DIVIDE MOD .
%right NOT .
%left SPACE .

%token_prefix ET_
%extra_argument { void* state }
%parse_failure { fprintf(stderr, "\033[1;32mParser failure... :(\033[0m\n"); }
%syntax_error { fprintf(stderr, "\033[1;31mSyntax error!\033[0m\n"); }
%include {#include <assert.h>}

expression ::= expr .

term ::= IDENTIFIER .
term ::= INTEGER .
term ::= FLOAT .
term ::= BOOLEAN .
term ::= STRING .

expr ::= term .

expr ::= SPACE expr .
expr ::= expr SPACE .
expr ::= expr PLUS expr .
expr ::= expr MINUS expr .
expr ::= expr DIVIDE expr .
expr ::= expr MOD expr .
expr ::= expr TIMES expr .
expr ::= LEFT_P expr RIGHT_P .
expr ::= MINUS expr . [NOT]
expr ::= expr LESS expr .
expr ::= expr LESS_OR_EQUAL expr .
expr ::= expr GREATER expr .
expr ::= expr GREATER_OR_EQUAL expr .
expr ::= expr EQUAL expr .
expr ::= expr NOT_EQUAL expr .
