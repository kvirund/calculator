#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>

namespace parser
{
enum EParserState
{
    EPS_UNDEFINED,
    EPS_ERROR,
    EPS_READY
};

class CParserImpl;

class CParser
{
    public:
        CParser(): m_state(EPS_UNDEFINED) {}
        bool parse(const std::string& expression);

    private:
        CParserImpl* impl();
        CParserImpl* m_impl;

        EParserState m_state;
};

class CCommonException: public std::exception
{
    public:
        CCommonException(const char* message): m_what(message) {}
        const char* what() const throw() { return m_what; }

    private:
        const char* m_what;
};

class CParserException: public CCommonException
{
    public:
        CParserException(const char* message): CCommonException(message) {}
};

class CRuntimeException: public CCommonException
{
    public:
        CRuntimeException(const char* message): CCommonException(message) {}
};

}

#endif
