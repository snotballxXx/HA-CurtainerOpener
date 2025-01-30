#ifndef _IMESSENGER_H_
#define _IMESSENGER_H_

#include <StreamString.h>
#include <PubSubClient.h>

namespace Interfaces
{
    class ITopicCallback;
    class IMessenger
    {
        public:
        virtual void sendMessage(const String& topic, const String &msg) = 0;
        virtual void subscribe(const String& topic, ITopicCallback* callback) = 0;
    };
}

#endif  //!_IMESSENGER_H_