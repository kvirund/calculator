/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 19 "grammar.y"

#include <assert.h>
#include "state.h"
#line 12 "grammar.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 31
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  void* 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  struct SState* state ;
#define ParseARG_PDECL , struct SState* state 
#define ParseARG_FETCH  struct SState* state  = yypParser->state 
#define ParseARG_STORE yypParser->state  = state 
#define YYNSTATE 48
#define YYNRULE 28
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (177)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    48,   47,   11,   10,    9,    8,    7,    6,    5,    4,
 /*    10 */    18,   16,   12,   14,   13,   17,   47,   11,   10,    9,
 /*    20 */     8,    7,    6,    5,    4,   18,   16,   12,   14,   13,
 /*    30 */    78,   47,   78,   18,   16,   12,   14,   13,   38,   47,
 /*    40 */    78,   11,   10,    9,    8,    7,    6,    5,    4,   18,
 /*    50 */    16,   12,   14,   13,   78,   47,   10,    9,    8,    7,
 /*    60 */     6,    5,    4,   18,   16,   12,   14,   13,   78,   47,
 /*    70 */     9,    8,    7,    6,    5,    4,   18,   16,   12,   14,
 /*    80 */    13,    2,   47,   78,   78,    1,   15,   46,   45,   44,
 /*    90 */    43,   42,    3,   77,   20,   37,   40,   41,   12,   14,
 /*   100 */    13,   78,   47,   30,   37,   40,   41,   21,   37,   40,
 /*   110 */    41,   78,   78,   31,   37,   40,   41,   39,   37,   40,
 /*   120 */    41,   36,   37,   40,   41,   35,   37,   40,   41,   34,
 /*   130 */    37,   40,   41,   22,   37,   40,   41,   23,   37,   40,
 /*   140 */    41,   29,   37,   40,   41,   28,   37,   40,   41,   27,
 /*   150 */    37,   40,   41,   26,   37,   40,   41,   25,   37,   40,
 /*   160 */    41,   24,   37,   40,   41,   19,   37,   40,   41,   33,
 /*   170 */    37,   40,   41,   32,   37,   40,   41,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,   16,    2,    3,    4,    5,    6,    7,    8,    9,
 /*    10 */    10,   11,   12,   13,   14,    1,   16,    2,    3,    4,
 /*    20 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    30 */    30,   16,   30,   10,   11,   12,   13,   14,   23,   16,
 /*    40 */    30,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*    50 */    11,   12,   13,   14,   30,   16,    3,    4,    5,    6,
 /*    60 */     7,    8,    9,   10,   11,   12,   13,   14,   30,   16,
 /*    70 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*    80 */    14,   11,   16,   30,   30,   15,   16,   17,   18,   19,
 /*    90 */    20,   21,   22,   25,   26,   27,   28,   29,   12,   13,
 /*   100 */    14,   30,   16,   26,   27,   28,   29,   26,   27,   28,
 /*   110 */    29,   30,   30,   26,   27,   28,   29,   26,   27,   28,
 /*   120 */    29,   26,   27,   28,   29,   26,   27,   28,   29,   26,
 /*   130 */    27,   28,   29,   26,   27,   28,   29,   26,   27,   28,
 /*   140 */    29,   26,   27,   28,   29,   26,   27,   28,   29,   26,
 /*   150 */    27,   28,   29,   26,   27,   28,   29,   26,   27,   28,
 /*   160 */    29,   26,   27,   28,   29,   26,   27,   28,   29,   26,
 /*   170 */    27,   28,   29,   26,   27,   28,   29,
};
#define YY_SHIFT_USE_DFLT (-16)
#define YY_SHIFT_COUNT (37)
#define YY_SHIFT_MIN   (-15)
#define YY_SHIFT_MAX   (86)
static const signed char yy_shift_ofst[] = {
 /*     0 */    70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
 /*    10 */    70,   70,   70,   70,   70,   70,   70,   70,   70,   15,
 /*    20 */     0,   39,   53,   66,   23,   23,   23,   23,   23,   23,
 /*    30 */    86,   86,  -15,  -15,  -15,  -15,  -15,   14,
};
#define YY_REDUCE_USE_DFLT (-1)
#define YY_REDUCE_COUNT (18)
#define YY_REDUCE_MIN   (0)
#define YY_REDUCE_MAX   (147)
static const short yy_reduce_ofst[] = {
 /*     0 */    68,  147,  143,  139,  135,  131,  127,  123,  119,  115,
 /*    10 */   111,  107,  103,   99,   95,   91,   87,   81,   77,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*    10 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*    20 */    76,   55,   68,   69,   75,   74,   73,   72,   71,   70,
 /*    30 */    60,   61,   67,   66,   64,   63,   62,   54,   65,   58,
 /*    40 */    57,   56,   53,   52,   51,   50,   49,   59,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "ASSIGN",        "AND",           "OR",          
  "LESS",          "LESS_OR_EQUAL",  "GREATER",       "GREATER_OR_EQUAL",
  "EQUAL",         "NOT_EQUAL",     "PLUS",          "MINUS",       
  "TIMES",         "DIVIDE",        "MOD",           "NOT",         
  "SPACE",         "IDENTIFIER",    "INTEGER",       "FLOAT",       
  "BOOLEAN",       "STRING",        "LEFT_P",        "RIGHT_P",     
  "error",         "expression",    "expr",          "identifier",  
  "term",          "variable",    
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "expression ::= expr",
 /*   1 */ "identifier ::= IDENTIFIER",
 /*   2 */ "term ::= INTEGER",
 /*   3 */ "term ::= FLOAT",
 /*   4 */ "term ::= BOOLEAN",
 /*   5 */ "term ::= STRING",
 /*   6 */ "term ::= identifier",
 /*   7 */ "variable ::= identifier ASSIGN expr",
 /*   8 */ "expr ::= variable",
 /*   9 */ "expr ::= term",
 /*  10 */ "expr ::= SPACE expr",
 /*  11 */ "expr ::= expr SPACE",
 /*  12 */ "expr ::= expr PLUS expr",
 /*  13 */ "expr ::= expr MINUS expr",
 /*  14 */ "expr ::= expr DIVIDE expr",
 /*  15 */ "expr ::= expr MOD expr",
 /*  16 */ "expr ::= expr TIMES expr",
 /*  17 */ "expr ::= LEFT_P expr RIGHT_P",
 /*  18 */ "expr ::= MINUS expr",
 /*  19 */ "expr ::= NOT expr",
 /*  20 */ "expr ::= expr AND expr",
 /*  21 */ "expr ::= expr OR expr",
 /*  22 */ "expr ::= expr LESS expr",
 /*  23 */ "expr ::= expr LESS_OR_EQUAL expr",
 /*  24 */ "expr ::= expr GREATER expr",
 /*  25 */ "expr ::= expr GREATER_OR_EQUAL expr",
 /*  26 */ "expr ::= expr EQUAL expr",
 /*  27 */ "expr ::= expr NOT_EQUAL expr",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* ASSIGN */
    case 2: /* AND */
    case 3: /* OR */
    case 4: /* LESS */
    case 5: /* LESS_OR_EQUAL */
    case 6: /* GREATER */
    case 7: /* GREATER_OR_EQUAL */
    case 8: /* EQUAL */
    case 9: /* NOT_EQUAL */
    case 10: /* PLUS */
    case 11: /* MINUS */
    case 12: /* TIMES */
    case 13: /* DIVIDE */
    case 14: /* MOD */
    case 15: /* NOT */
    case 16: /* SPACE */
    case 17: /* IDENTIFIER */
    case 18: /* INTEGER */
    case 19: /* FLOAT */
    case 20: /* BOOLEAN */
    case 21: /* STRING */
    case 22: /* LEFT_P */
    case 23: /* RIGHT_P */
{
#line 23 "grammar.y"
 
#line 452 "grammar.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 25, 1 },
  { 27, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 29, 3 },
  { 26, 1 },
  { 26, 1 },
  { 26, 2 },
  { 26, 2 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 2 },
  { 26, 2 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* expression ::= expr */
#line 25 "grammar.y"
{ state->root = yymsp[0].minor.yy0; }
#line 775 "grammar.c"
        break;
      case 1: /* identifier ::= IDENTIFIER */
#line 28 "grammar.y"
{
        yygotominor.yy0 = create_identifier(yymsp[0].minor.yy0, state->vpool);
        add_to_temp_set(yygotominor.yy0, state->temp_set);
    }
#line 783 "grammar.c"
        break;
      case 2: /* term ::= INTEGER */
#line 34 "grammar.y"
{
        yygotominor.yy0 = create_integer(yymsp[0].minor.yy0);
        add_to_temp_set(yygotominor.yy0, state->temp_set);
    }
#line 791 "grammar.c"
        break;
      case 3: /* term ::= FLOAT */
#line 39 "grammar.y"
{
        yygotominor.yy0 = create_float(yymsp[0].minor.yy0);
        add_to_temp_set(yygotominor.yy0, state->temp_set);
    }
#line 799 "grammar.c"
        break;
      case 4: /* term ::= BOOLEAN */
#line 44 "grammar.y"
{
        yygotominor.yy0 = create_boolean(yymsp[0].minor.yy0);
        add_to_temp_set(yygotominor.yy0, state->temp_set);
    }
#line 807 "grammar.c"
        break;
      case 5: /* term ::= STRING */
#line 49 "grammar.y"
{
        yygotominor.yy0 = create_string(yymsp[0].minor.yy0);
        add_to_temp_set(yygotominor.yy0, state->temp_set);
    }
#line 815 "grammar.c"
        break;
      case 6: /* term ::= identifier */
      case 8: /* expr ::= variable */ yytestcase(yyruleno==8);
      case 9: /* expr ::= term */ yytestcase(yyruleno==9);
#line 53 "grammar.y"
{ yygotominor.yy0 = yymsp[0].minor.yy0; }
#line 822 "grammar.c"
        break;
      case 7: /* variable ::= identifier ASSIGN expr */
#line 56 "grammar.y"
{
        yygotominor.yy0 = evaluate(yymsp[-2].minor.yy0, yymsp[0].minor.yy0); // yygotominor.yy0 will be point to yymsp[-2].minor.yy0

        // evaluated expression should be destroyed
        remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
        delete_node(yymsp[0].minor.yy0);
      yy_destructor(yypParser,1,&yymsp[-1].minor);
}
#line 834 "grammar.c"
        break;
      case 10: /* expr ::= SPACE expr */
#line 67 "grammar.y"
{ yygotominor.yy0 = yymsp[0].minor.yy0;   yy_destructor(yypParser,16,&yymsp[-1].minor);
}
#line 840 "grammar.c"
        break;
      case 11: /* expr ::= expr SPACE */
#line 68 "grammar.y"
{yygotominor.yy0 = yymsp[-1].minor.yy0;   yy_destructor(yypParser,16,&yymsp[0].minor);
}
#line 846 "grammar.c"
        break;
      case 12: /* expr ::= expr PLUS expr */
#line 70 "grammar.y"
{
    yygotominor.yy0 = create_add_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,10,&yymsp[-1].minor);
}
#line 857 "grammar.c"
        break;
      case 13: /* expr ::= expr MINUS expr */
#line 77 "grammar.y"
{
    yygotominor.yy0 = create_sub_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 868 "grammar.c"
        break;
      case 14: /* expr ::= expr DIVIDE expr */
#line 85 "grammar.y"
{
    yygotominor.yy0 = create_div_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,13,&yymsp[-1].minor);
}
#line 879 "grammar.c"
        break;
      case 15: /* expr ::= expr MOD expr */
#line 92 "grammar.y"
{
    yygotominor.yy0 = create_mod_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,14,&yymsp[-1].minor);
}
#line 890 "grammar.c"
        break;
      case 16: /* expr ::= expr TIMES expr */
#line 99 "grammar.y"
{
    yygotominor.yy0 = create_times_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,12,&yymsp[-1].minor);
}
#line 901 "grammar.c"
        break;
      case 17: /* expr ::= LEFT_P expr RIGHT_P */
#line 106 "grammar.y"
{
    yygotominor.yy0 = yymsp[-1].minor.yy0;
  yy_destructor(yypParser,22,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 910 "grammar.c"
        break;
      case 18: /* expr ::= MINUS expr */
#line 110 "grammar.y"
{
    yygotominor.yy0 = create_minus_operator(yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 920 "grammar.c"
        break;
      case 19: /* expr ::= NOT expr */
#line 116 "grammar.y"
{
    yygotominor.yy0 = create_not_operator(yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,15,&yymsp[-1].minor);
}
#line 930 "grammar.c"
        break;
      case 20: /* expr ::= expr AND expr */
#line 122 "grammar.y"
{
    yygotominor.yy0 = create_and_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,2,&yymsp[-1].minor);
}
#line 941 "grammar.c"
        break;
      case 21: /* expr ::= expr OR expr */
#line 129 "grammar.y"
{
    yygotominor.yy0 = create_or_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 952 "grammar.c"
        break;
      case 22: /* expr ::= expr LESS expr */
#line 136 "grammar.y"
{
    yygotominor.yy0 = create_less_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 963 "grammar.c"
        break;
      case 23: /* expr ::= expr LESS_OR_EQUAL expr */
#line 143 "grammar.y"
{
    yygotominor.yy0 = create_less_or_equal_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,5,&yymsp[-1].minor);
}
#line 974 "grammar.c"
        break;
      case 24: /* expr ::= expr GREATER expr */
#line 150 "grammar.y"
{
    yygotominor.yy0 = create_greater_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 985 "grammar.c"
        break;
      case 25: /* expr ::= expr GREATER_OR_EQUAL expr */
#line 157 "grammar.y"
{
    yygotominor.yy0 = create_greater_or_equal_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 996 "grammar.c"
        break;
      case 26: /* expr ::= expr EQUAL expr */
#line 164 "grammar.y"
{
    yygotominor.yy0 = create_equal_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,8,&yymsp[-1].minor);
}
#line 1007 "grammar.c"
        break;
      case 27: /* expr ::= expr NOT_EQUAL expr */
#line 171 "grammar.y"
{
    yygotominor.yy0 = create_not_equal_operator(yymsp[-2].minor.yy0, yymsp[0].minor.yy0);
    add_to_temp_set(yygotominor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[-2].minor.yy0, state->temp_set);
    remove_from_temp_set(yymsp[0].minor.yy0, state->temp_set);
  yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 1018 "grammar.c"
        break;
      default:
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 13 "grammar.y"

    state->syntax_error = 1;
#line 1069 "grammar.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 16 "grammar.y"

    state->syntax_error = 1;
#line 1087 "grammar.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
