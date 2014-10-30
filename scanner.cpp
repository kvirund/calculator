/* Generated by re2c 0.13.5 on Thu Oct 30 23:26:16 2014 */
#line 1 "scanner.re"
#include "scanner.h"

namespace scanner
{

CToken scan(const std::string& expression, size_t& offset)
{
    std::string::const_pointer marker;
    std::string::const_pointer cursor = expression.c_str() + offset;
    std::string::const_pointer p_start = expression.c_str() + offset;
    size_t start = offset;


#line 17 "scanner.cpp"
    {
        unsigned char yych;
        unsigned int yyaccept = 0;

        yych = (unsigned char)*cursor;
        switch (yych) {
        case 0x00:    goto yy35;
        case '\t':
        case ' ':    goto yy3;
        case '!':    goto yy20;
        case '"':    goto yy9;
        case '%':    goto yy27;
        case '(':    goto yy31;
        case ')':    goto yy33;
        case '*':    goto yy29;
        case '+':    goto yy21;
        case '-':    goto yy23;
        case '/':    goto yy25;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy7;
        case '<':    goto yy15;
        case '=':    goto yy19;
        case '>':    goto yy17;
        case 'A':
        case 'a':    goto yy11;
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':    goto yy12;
        case 'F':
        case 'f':    goto yy6;
        case 'N':
        case 'n':    goto yy13;
        case 'O':
        case 'o':    goto yy14;
        case 'T':
        case 't':    goto yy4;
        default:    goto yy37;
        }
yy2:
#line 48 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_SPACE, expression.substr(start, cursor - p_start)); }
#line 107 "scanner.cpp"
yy3:
        yych = (unsigned char)*++cursor;
        goto yy75;
yy4:
        ++cursor;
        switch ((yych = (unsigned char)*cursor)) {
        case 'R':
        case 'r':    goto yy72;
        default:    goto yy48;
        }
yy5:
#line 53 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_IDENTIFIER, expression.substr(start, cursor - p_start)); }
#line 121 "scanner.cpp"
yy6:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'A':
        case 'a':    goto yy67;
        default:    goto yy48;
        }
yy7:
        yyaccept = 0;
        yych = (unsigned char)*(marker = ++cursor);
        switch (yych) {
        case '.':    goto yy61;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy59;
        default:    goto yy62;
        }
yy8:
#line 50 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_FLOAT, expression.substr(start, cursor - p_start)); }
#line 149 "scanner.cpp"
yy9:
        yyaccept = 1;
        yych = (unsigned char)*(marker = ++cursor);
        goto yy54;
yy10:
#line 76 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_ERROR, expression.substr(start, cursor - p_start)); }
#line 157 "scanner.cpp"
yy11:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'N':
        case 'n':    goto yy51;
        default:    goto yy48;
        }
yy12:
        yych = (unsigned char)*++cursor;
        goto yy48;
yy13:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'O':
        case 'o':    goto yy49;
        default:    goto yy48;
        }
yy14:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'R':
        case 'r':    goto yy46;
        default:    goto yy48;
        }
yy15:
        ++cursor;
        switch ((yych = (unsigned char)*cursor)) {
        case '=':    goto yy44;
        default:    goto yy16;
        }
yy16:
#line 55 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_LESS, expression.substr(start, cursor - p_start)); }
#line 191 "scanner.cpp"
yy17:
        ++cursor;
        switch ((yych = (unsigned char)*cursor)) {
        case '=':    goto yy42;
        default:    goto yy18;
        }
yy18:
#line 56 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_GREATER, expression.substr(start, cursor - p_start)); }
#line 201 "scanner.cpp"
yy19:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case '=':    goto yy40;
        default:    goto yy10;
        }
yy20:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case '=':    goto yy38;
        default:    goto yy10;
        }
yy21:
        ++cursor;
#line 66 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_PLUS, expression.substr(start, cursor - p_start)); }
#line 218 "scanner.cpp"
yy23:
        ++cursor;
#line 67 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_MINUS, expression.substr(start, cursor - p_start)); }
#line 223 "scanner.cpp"
yy25:
        ++cursor;
#line 69 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_DIVIDE, expression.substr(start, cursor - p_start)); }
#line 228 "scanner.cpp"
yy27:
        ++cursor;
#line 70 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_MOD, expression.substr(start, cursor - p_start)); }
#line 233 "scanner.cpp"
yy29:
        ++cursor;
#line 71 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_TIMES, expression.substr(start, cursor - p_start)); }
#line 238 "scanner.cpp"
yy31:
        ++cursor;
#line 73 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_LEFT_P, expression.substr(start, cursor - p_start)); }
#line 243 "scanner.cpp"
yy33:
        ++cursor;
#line 74 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_RIGHT_P, expression.substr(start, cursor - p_start)); }
#line 248 "scanner.cpp"
yy35:
        ++cursor;
#line 75 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_EOF, expression.substr(start, cursor - p_start)); }
#line 253 "scanner.cpp"
yy37:
        yych = (unsigned char)*++cursor;
        goto yy10;
yy38:
        ++cursor;
#line 60 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_NOT_EQUAL, expression.substr(start, cursor - p_start)); }
#line 261 "scanner.cpp"
yy40:
        ++cursor;
#line 59 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_EQUAL, expression.substr(start, cursor - p_start)); }
#line 266 "scanner.cpp"
yy42:
        ++cursor;
#line 58 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_GREATER_OR_EQUAL, expression.substr(start, cursor - p_start)); }
#line 271 "scanner.cpp"
yy44:
        ++cursor;
#line 57 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_LESS_OR_EQUAL, expression.substr(start, cursor - p_start)); }
#line 276 "scanner.cpp"
yy46:
        yych = (unsigned char)*++cursor;
        goto yy48;
yy47:
        ++cursor;
        yych = (unsigned char)*cursor;
yy48:
        switch (yych) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':    goto yy47;
        default:    goto yy5;
        }
yy49:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'T':
        case 't':    goto yy50;
        default:    goto yy48;
        }
yy50:
        yych = (unsigned char)*++cursor;
        goto yy48;
yy51:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'D':
        case 'd':    goto yy52;
        default:    goto yy48;
        }
yy52:
        yych = (unsigned char)*++cursor;
        goto yy48;
yy53:
        ++cursor;
        yych = (unsigned char)*cursor;
yy54:
        switch (yych) {
        case '"':    goto yy57;
        case '\\':    goto yy55;
        default:    goto yy53;
        }
yy55:
        ++cursor;
        yych = (unsigned char)*cursor;
        switch (yych) {
        case '"':
        case '\\':    goto yy53;
        default:    goto yy56;
        }
yy56:
        cursor = marker;
        switch (yyaccept) {
        case 0: 	goto yy8;
        case 1: 	goto yy10;
        }
yy57:
        ++cursor;
#line 52 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_STRING, expression.substr(1 + start, cursor - p_start - 2)); }
#line 397 "scanner.cpp"
yy59:
        yyaccept = 0;
        marker = ++cursor;
        yych = (unsigned char)*cursor;
        switch (yych) {
        case '.':    goto yy61;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy59;
        case 'E':
        case 'e':    goto yy63;
        default:    goto yy8;
        }
yy61:
        yyaccept = 0;
        marker = ++cursor;
        yych = (unsigned char)*cursor;
yy62:
        switch (yych) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy61;
        case 'E':
        case 'e':    goto yy63;
        default:    goto yy8;
        }
yy63:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case '+':
        case '-':    goto yy64;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy65;
        default:    goto yy56;
        }
yy64:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy65;
        default:    goto yy56;
        }
yy65:
        ++cursor;
        yych = (unsigned char)*cursor;
        switch (yych) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':    goto yy65;
        default:    goto yy8;
        }
yy67:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'L':
        case 'l':    goto yy68;
        default:    goto yy48;
        }
yy68:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'S':
        case 's':    goto yy69;
        default:    goto yy48;
        }
yy69:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'E':
        case 'e':    goto yy70;
        default:    goto yy48;
        }
yy70:
        ++cursor;
        switch ((yych = (unsigned char)*cursor)) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':    goto yy47;
        default:    goto yy71;
        }
yy71:
#line 49 "scanner.re"
        { offset += cursor - p_start; return CToken(ET_BOOLEAN, expression.substr(start, cursor - p_start)); }
#line 578 "scanner.cpp"
yy72:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'U':
        case 'u':    goto yy73;
        default:    goto yy48;
        }
yy73:
        yych = (unsigned char)*++cursor;
        switch (yych) {
        case 'E':
        case 'e':    goto yy70;
        default:    goto yy48;
        }
yy74:
        ++cursor;
        yych = (unsigned char)*cursor;
yy75:
        switch (yych) {
        case '\t':
        case ' ':    goto yy74;
        default:    goto yy2;
        }
    }
#line 78 "scanner.re"

}
}
