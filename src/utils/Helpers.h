#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <StreamString.h>
#include "./control/State.h"

namespace Utils
{
    class Helpers
    {
       public:
        static String composeClientID();

        static String getValue(String data, char separator, int index);

        static String stateToString(Control::State state);
    };
}  // namespace Utils

#endif  //_HELPERS_H_