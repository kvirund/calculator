enum EToken
{
    ET_ERROR = -1,
    ET_IDENTIFIER,
    ET_OPERATION,
    ET_BOOLEAN,
    ET_INTEGER,
    ET_FLOAT,
    ET_STRING,
    ET_LEFT_PARENTHESIS,
    ET_RIGHT_PARENTHESIS,
    ET_EOL,
    ET_END
};

EToken scan(const char **p, const char **lex)
{
    char *marker = marker;
    if (!p)
    {
        return ET_ERROR;
    }

    if (lex)
    {
        *lex = *p;
    }

/*!re2c

re2c:define:YYCTYPE = "EToken";
re2c:define:YYCURSOR = *p;
re2c:define:YYMARKER = marker;
re2c:yyfill:enable = 0;
re2c:yych:conversion = 1;
re2c:indent:top = 1;

IDENTIFIER = [a-zA-Z_][a-zA-Z0-9_]*
OPERATION = '<' | '>' | '<=' | '>=' | '==' | '+' | '-' | '/' | '*'
BOOLEAN = 'true' | 'false'
INTEGER = [0-9]+
FLOAT = [0-9]+ ('.' [0-9]*)? ('e'[+-][0-9]+)?
STRING = '"' ([^"\\]|\\|\")* '"'
LEFT_P = '('
RIGHT_P = ')'
SPACE = [ \t]*
EOL = '\n'
END = '\0'

SPACE       { }
BOOLEAN     { return ET_BOOLEAN; }
FLOAT       { return ET_FLOAT; }
INTEGER     { return ET_INTEGER; }
STRING      { return ET_STRING; }
IDENTIFIER  { return ET_IDENTIFIER; }
OPERATION   { return ET_OPERATION; }
LEFT_P      { return ET_LEFT_PARENTHESIS; }
RIGHT_P     { return ET_RIGHT_PARENTHESIS; }
EOL         { return ET_EOL; }
END         { return ET_END; }

*/
}
