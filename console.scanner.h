#ifndef __CONSOLE_SCANNER_H__
#define __CONSOLE_SCANNER_H__

#include "parser_utils.h"

namespace scanner
{
    namespace console
    {
        class CScanner: public CBaseScanner
        {
            public:
                CToken operator()(const std::string& expression, CState& state) const;
        };
    }
}

#endif // __CONSOLE_SCANNER_H__
