#ifndef __COLORS_H__
#define __COLORS_H__

namespace colors
{
    extern const char* RESET;
    extern const char* RED;
    extern const char* GREEN;
    extern const char* BLUE;
    extern bool enabled;

    inline void turn_on() { enabled = true; };
    inline void turn_off() { enabled = false; };

    inline const char* reset() { return enabled ? RESET : ""; }
    inline const char* green() { return enabled ? GREEN : ""; }
    inline const char* red() { return enabled ? RED : ""; }
    inline const char* blue() { return enabled ? BLUE : ""; }
}

#endif
/* vim: set ts=4 sw=4 tw=0 et syntax=cpp :*/
