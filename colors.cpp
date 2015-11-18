#include "colors.h"

#define C_RESET "\033[0m"
#define C_RED "\033[1;31m"
#define C_GREEN "\033[0;32m"
#define C_BLUE "\033[1;34m"

namespace colors
{
    const char* RESET = C_RESET;
    const char* RED = C_RED;
    const char* GREEN = C_GREEN;
    const char* BLUE = C_BLUE;

#if defined USE_COLORS
    bool enabled = true;
#else
    bool enabled = false;
#endif
}
