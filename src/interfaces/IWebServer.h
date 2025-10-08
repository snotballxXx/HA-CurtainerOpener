#ifndef _IWEBSERVER_H_
#define _IWEBSERVER_H_
#include "./IArduinoBase.h"
#include <ESP8266WebServer.h>
#include "ITopicCallback.h"
#include "ILogger.h"

namespace Interfaces
{
    class IWebServer : public IAduninoBase
    {
       public:
        virtual ESP8266WebServer*           getServer()   = 0;
        virtual Interfaces::ITopicCallback* getCallback() = 0;
        virtual int getConnectedWebSocketCount() = 0;
        virtual void broadcastTXT(const String& msg) = 0;

        virtual int  getSlider()          = 0;
        virtual void setSlider(int value) = 0;
    };
}  // namespace Interfaces

#endif  //!_IWEBSERVER_H_