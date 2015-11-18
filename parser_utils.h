#ifndef __PARSER_UTILS_H__
#define __PARSER_UTILS_H__

#include <string>

namespace scanner
{
    typedef int EToken;

    std::string unescape_string(const std::string& s, const bool do_it);

    class CState
    {
        public:
            typedef int condition_t;

            CState(): m_condition(0), m_offset(0) {}
            condition_t condition() const { return m_condition; }
            void condition(const condition_t condition) { m_condition = condition; }
            size_t offset() const { return m_offset; }
            void move(const size_t offset) { m_offset += offset; }

        private:
            condition_t m_condition;
            size_t m_offset;
    };

    class CToken
    {
        public:
            const static int ET_EOF;
            const static int ET_ERROR;
            const static int ET_END;

            CToken(): m_token(ET_ERROR), m_value("uninitialized") {};
            CToken(EToken token, const std::string& value): m_token(token), m_value(value) {}
            CToken(EToken token, const std::string& value, const bool unescape): m_token(token), m_value(unescape_string(value, unescape)) {}
            EToken token() const { return m_token; }
            const std::string& value() const { return m_value; }

        private:
            EToken m_token;
            std::string m_value;
    };

    class CBaseScanner
    {
        protected:
            CBaseScanner() {}

        public:
            CToken operator()(const std::string& expression, CState& state);
    };
}

#include <iostream>

namespace parser
{
    namespace utils
    {
        // Implementation taken from http://www.drdobbs.com/architecture-and-design/matching-wildcards-an-empirical-way-to-t/240169123
        inline
        bool WildTextCompare(const char *pTameText, const char *pWildText)
        {
            const char *pTameBookmark = (char *) 0;
            const char *pWildBookmark = (char *) 0;

            while (1)
            {
                if (*pWildText == '%')
                {
                    while (*(++pWildText) == '%')
                    {
                    }

                    if (!*pWildText)
                    {
                        return true;
                    }

                    if (*pWildText != '_')
                    {
                        while (*pTameText != *pWildText)
                        {
                            if (!(*(++pTameText)))
                                return false;
                        }
                    }

                    pWildBookmark = pWildText;
                    pTameBookmark = pTameText;
                }
                else if (*pTameText != *pWildText && *pWildText != '_')
                {
                    if (pWildBookmark)
                    {
                        if (pWildText != pWildBookmark)
                        {
                            pWildText = pWildBookmark;

                            if (*pTameText != *pWildText)
                            {
                                pTameText = ++pTameBookmark;
                                continue;
                            }
                            else
                            {
                                pWildText++;
                            }
                        }

                        if (*pTameText)
                        {
                            pTameText++;
                            continue;
                        }
                    }

                    return false;
                }

                pTameText++;
                pWildText++;

                if (!*pTameText)
                {
                    while (*pWildText == '%')
                    {
                        pWildText++;
                    }

                    if (!*pWildText)
                    {
                        return true;
                    }

                    return false;
                }
            }
        }
    }
}

#endif
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/
