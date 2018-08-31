
/*
This file is part of OGS Mahjong components:
  https://github.com/OGStudio/ogs-mahjong-components

Copyright (C) 2018 Opensource Game Studio

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef OGS_MAHJONG_COMPONENTS_FORMAT_H
#define OGS_MAHJONG_COMPONENTS_FORMAT_H

// printfString Start
#include <cstdarg>

// printfString End

namespace omc
{
namespace format
{

// printfString Start
//! Construct a string using printf-like syntax.
std::string printfString(const char *fmt, ...)
{
    const int PRINTF_STRING_MAX_STRLEN = 1024;
    va_list args;
    char msg[PRINTF_STRING_MAX_STRLEN];
    va_start(args, fmt);
    vsnprintf(msg, PRINTF_STRING_MAX_STRLEN, fmt, args);
    va_end(args);
    return msg;
}
// printfString End
// stringStartsWith Start
//! Find prefix in the provided string.
bool stringStartsWith(const std::string &s, const std::string &prefix)
{
    // Source: https://stackoverflow.com/a/8095127
    // Topic: how to check string start in C++
    return
        (prefix.length() <= s.length()) &&
        std::equal(prefix.begin(), prefix.end(), s.begin())
        ;
}
// stringStartsWith End
// trimmedString Start
//! Trim non-visible characters at the beginning and at the end.
std::string trimmedString(const std::string &s)
{
    // Source: https://stackoverflow.com/a/21698913
    // Topic: What's the best way to trim std::string?

    // Find the first position without space characters.
    std::string::const_iterator it = s.begin();
    while (
        (it != s.end()) &&
        std::isspace(*it)
    ) {
        ++it;
    }

    // Find the last position without space characters.
    std::string::const_reverse_iterator rit = s.rbegin();
    while (
        (rit.base() != it) &&
        std::isspace(*rit)
    ) {
        ++rit;
    }

    return std::string(it, rit.base());
}
// trimmedString End


} // namespace format
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_FORMAT_H

