#ifndef COMMON_H
#define COMMON_H

#include "Configuration.h"

std::string string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

//std::string string_format(const std::string fmt, ...)
String String_Format( const String fmt , ... )
{
	int size = ( (int) fmt.length() ) + 32; //* 2 + 50;   // Use a rubric appropriate for your code
	String str;
	va_list ap;
	while (1) // Maximum two passes on a POSIX system...
	{     
		if( ! str.reserve( size ) )
		{
			break;
		}
		va_start( ap , fmt );
		// Notice that only when this returned value is non-negative and less than n, the string has been completely written.
		int n = vsnprintf( (char *) str.begin() , size , fmt.c_str() , ap );
		va_end( ap );
		if ( n > -1 && n < size )
		{
			// Everything worked
			str.reserve( n );
			return str;
		}
		if ( n > -1 )
		{
			// Needed size returned
			size = n + 1;   // For null char
		}
		else
		{
			// Guess at a larger size (OS specific) => attempt to crash...
			size *= 2;
		}
	}
	return str;
}

/*
// Expose Espressif SDK functionality - wrapped in ifdef so that it still
// compiles on other platforms
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif
*/


#endif //COMMON_H