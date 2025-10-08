#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "../interfaces/IWebServer.h"
#include <ESP8266WebServer.h>
#include "./FileSystem.h"
#include "interfaces/ITopicCallback.h"
#include "interfaces/IMessenger.h"
#include <WebSocketsServer.h>

namespace Control
{
    class WebServer : public Interfaces::IWebServer
    {
        ESP8266WebServer*           _webServer;
        WebSocketsServer*           _webSocket;
        FileSystem*                 _fileSystem;
        Interfaces::ITopicCallback* _actionCallback;
        int                         _slider;
        Interfaces::IMessenger*     _messenger;

       public:
        WebServer(FileSystem* fileSystem, Interfaces::ITopicCallback* actionCallback,
                  Interfaces::IMessenger* messenger);
        virtual void loop(unsigned long time);
        virtual void setup();

        virtual ESP8266WebServer*           getServer() { return _webServer; };
        virtual Interfaces::ITopicCallback* getCallback() { return _actionCallback; }
        virtual int getConnectedWebSocketCount();
        virtual void broadcastTXT(const String& msg);

        virtual int  getSlider() { return _slider; }
        virtual void setSlider(int value) { _slider = value; }
    };
}  // namespace Control

#endif  //!_WEBSERVER_H_