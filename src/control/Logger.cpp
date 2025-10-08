#include "./Logger.h"
#include "./Repository.h"
#include "../interfaces/ITimeServer.h"

using namespace Control;

Logger::Logger(Interfaces::IWebServer* webserver, Interfaces::ITimeServer* timeServer,
               Interfaces::IMessenger* messenger)
    : _webServer(webserver), _timeServer(timeServer), _messenger(messenger)
{
}

void Logger::sendLog(const String& txt)
{
    Serial.println(txt);
    if (_webServer->getConnectedWebSocketCount() != 0 || Repository::getInstance()->getLogToMq() == 1)
    {
        String msg(_timeServer->getTimeAsString(millis()) + " [" + Repository::getInstance()->getEntityId() + "] " +
                   txt);

        if (_webServer->getConnectedWebSocketCount() != 0) _webServer->broadcastTXT(msg);

        if (Repository::getInstance()->getLogToMq() == 1)
        {
            Serial.println(msg);
            _messenger->sendMessage("logger/" + Repository::getInstance()->getEntityId(), msg);
        }
    }
}
