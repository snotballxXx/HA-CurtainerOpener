#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "../interfaces/IWebServer.h"
#include "interfaces/ILogger.h"
#include "interfaces/ITimeServer.h"
#include "interfaces/IMessenger.h"

namespace Control
{
    class Logger : public Interfaces::ILogger
    {
        Interfaces::IWebServer*  _webServer;
        Interfaces::ITimeServer* _timeServer;
        Interfaces::IMessenger*  _messenger;

       public:
        Logger(Interfaces::IWebServer* webserver, Interfaces::ITimeServer* timeServer, Interfaces::IMessenger*  messenger);
        virtual void sendLog(const String& txt);
    };
}  // namespace Control

#endif  //!_LOGGER_H_