#ifndef __EXPRESSIONS_SCANNER_H__
#define __EXPRESSIONS_SCANNER_H__

#include "parser_utils.h"

#include "expressions.grammar.h"

#include <string>

namespace scanner
{
    namespace expressions
    {
        class CScanner: public CBaseScanner
        {
            public:
                CToken operator()(const std::string& expression, CState& state) const;
        };
    }
}

#endif
