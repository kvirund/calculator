#include "parser_utils.h"

namespace scanner
{
    const int CToken::ET_EOF = -2;
    const int CToken::ET_ERROR = -1;
    const int CToken::ET_END = 0;

    std::string unescape_string(const std::string& s, const bool do_it)
    {
        std::string result(s);
        if (do_it)
        {
            size_t j = 0;
            for (size_t i = 0; i < result.size(); ++i, ++j)
            {
                if ('\\' == result[i])
                {
                    if (1 + i < result.size())
                    {
                        ++i;
                    }
                }
                result[j] = result[i];
            }
            result.resize(j);
        }
        return result;
    }
}
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/
