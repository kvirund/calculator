#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>

namespace scanner
{
#define TOKENS TOKEN(ET_ERROR) \
    TOKEN(ET_IDENTIFIER) \
    TOKEN(ET_OPERATION) \
    TOKEN(ET_BOOLEAN) \
    TOKEN(ET_INTEGER) \
    TOKEN(ET_FLOAT) \
    TOKEN(ET_STRING) \
    TOKEN(ET_LEFT_PARENTHESIS) \
    TOKEN(ET_RIGHT_PARENTHESIS) \
    TOKEN(ET_SPACE) \
    TOKEN(ET_EOL) \
    TOKEN(ET_END)

    extern const char* TOKEN_NAMES[];

    enum EToken
    {
#define TOKEN(x) x,
        TOKENS
#undef TOKEN
    };

    class CToken
    {
        public:
            CToken(): m_token(ET_ERROR), m_value("uninitialized") {}
            CToken(EToken token, const std::string& value): m_token(token), m_value(value) {}
            EToken token() const { return m_token; }
            const std::string& value() const { return m_value; }

        private:
            EToken m_token;
            std::string m_value;
    };

    CToken scan(const std::string& expression, size_t& offset);
}

#endif
