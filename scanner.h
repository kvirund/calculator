#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>

namespace scanner
{
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
