#ifndef _ITIME_SERVER_H_
#define _ITIME_SERVER_H_
#include "./IArduinoBase.h"
#include <StreamString.h>

namespace Interfaces
{
    class ITimeServer : public IAduninoBase
    {
       public:
        virtual unsigned long getTime()                                  = 0;
        virtual String        getTimeAsString(unsigned long requestTime) = 0;
    };
}  // namespace Interfaces

#endif  //!_ITIME_SERVER_H_