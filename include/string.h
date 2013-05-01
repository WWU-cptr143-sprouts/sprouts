/*
 * Garrett added this. It's fast enough.
 */

#ifndef H_TOSTRING
#define H_TOSTRING

#include <string>
#include <sstream>

template<class T> std::string to_string(T t)
{
    std::ostringstream s;
    s << t;
    return s.str();
}

#endif
