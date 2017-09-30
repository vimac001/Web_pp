/* 
 * File:   string.h
 * Author: vikto
 *
 * Created on 8. August 2015, 21:24
 */

#include <string>


#ifndef STRING_H
#define STRING_H
#if defined(__MINGW32__) || defined(__CYGWIN__)

namespace std
{
    string to_string (int val);
    string to_string (long val);
    string to_string (long long val);
    string to_string (unsigned val);
    string to_string (unsigned long val);
    string to_string (unsigned long long val);
    string to_string (float val);
    string to_string (double val);
    string to_string (long double val);
}

#endif

namespace wpp
{
	const char		  NULLCHR		= '\0';
	const std::string EMPTY_STRING	= "";
}

#endif /* STRING_H */