#ifndef __SCANNER_H__
#define __SCANNER_H__

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
    ET_SPACE,
    ET_EOL,
    ET_END
};

EToken scan(const char **p, const char **lex);

#endif
