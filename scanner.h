#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "grammar.h"

#include <string>

namespace scanner
{
#define ET_EOF -2
#define ET_ERROR -1
#define ET_END 0

    typedef int EToken;

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
