#ifndef __WHERE_SCANNER_H__
#define __WHERE_SCANNER_H__

#include "parser_utils.h"

#include "where.grammar.h"

#include <string>

namespace scanner
{
    namespace where
    {
        class CScanner: public CBaseScanner
        {
            public:
                CToken operator()(const std::string& expression, CState& state) const;
        };
    }
}

#endif
