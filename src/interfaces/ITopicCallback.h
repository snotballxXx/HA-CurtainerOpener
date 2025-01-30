#ifndef _ITOPIC_CALLBACK_H_
#define _ITOPIC_CALLBACK_H_
#include "./IArduinoBase.h"
#include <StreamString.h>

namespace Interfaces
{
    class ITopicCallback
    {
        public:
        virtual void messageReceived(const String& topic, const String& payload) = 0;
    };
}

#endif  //!_ITOPIC_CALLBACK_H_