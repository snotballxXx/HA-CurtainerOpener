#ifndef _IWEBSERVER_H_
#define _IWEBSERVER_H_
#include "./IArduinoBase.h"
#include <ESP8266WebServer.h>
#include "ITopicCallback.h"

namespace Interfaces
{
    class IWebServer : public IAduninoBase
    {
        public:
        virtual ESP8266WebServer* getServer() = 0;
        virtual Interfaces::ITopicCallback* getCallback() = 0;
    };
}

#endif  //!_IWEBSERVER_H_