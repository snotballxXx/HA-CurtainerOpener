#ifndef _ILOGGER_H_
#define _ILOGGER_H_
#include <StreamString.h>

namespace Interfaces
{
    class ILogger
    {
       public:
        virtual void sendLog(const String& txt) = 0;
    };
}  // namespace Interfaces

#endif  //!_ILOGGER_H_