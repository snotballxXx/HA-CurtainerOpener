#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <StreamString.h>

namespace Utils
{
    class Helpers
    {
    public:
        static String composeClientID();

        static String getValue(String data, char separator, int index);
    };
}

#endif //_HELPERS_H_