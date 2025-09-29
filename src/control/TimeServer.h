#ifndef _TIMESERVER_H_
#define _TIMESERVER_H_

#include "../interfaces/ITimeServer.h"
#include "../interfaces/ITopicCallback.h"
#include "../interfaces/IMessenger.h"
#include <StreamString.h>

namespace Control
{
    class TimeServer : public Interfaces::ITimeServer, public Interfaces::ITopicCallback
    {
        Interfaces::IMessenger *_messenger;
        uint64_t _time;
        uint64_t _lastUpdateRequest;
        uint64_t _currentTime;
        uint64_t _timeOffset;
        uint64_t _lastRequestTime;
        uint64_t _baseTime;
        String _lastRequestAsString;

    public:
        TimeServer(Interfaces::IMessenger *messenger);
        virtual void loop(unsigned long time);
        virtual void setup();
        virtual unsigned long getTime();
        virtual String getTimeAsString(unsigned long requestTime);
        virtual void messageReceived(const String &topic, const String &payload);
    };
}

#endif //!_TIMESERVER_H_
