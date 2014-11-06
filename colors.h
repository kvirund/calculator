#ifndef __COLORS_H__
#define __COLORS_H__

#ifdef USE_COLORS
#   define RESET "\033[0m"
#   define GREEN "\033[0;32m"
#   define RED "\033[1;31m"
#   define BLUE "\033[1;34m"
#else
#   define RESET ""
#   define GREEN ""
#   define RED ""
#   define BLUE ""
#endif

#endif
