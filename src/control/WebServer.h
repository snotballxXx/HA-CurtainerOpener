#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "../interfaces/IWebServer.h"
#include <ESP8266WebServer.h>
#include "./FileSystem.h"
#include <interfaces/ITopicCallback.h>

namespace Control
{
    class WebServer : public Interfaces::IWebServer
    {
        ESP8266WebServer* _webServer;
        FileSystem *_fileSystem;
        Interfaces::ITopicCallback* _actionCallback;

        public:
        WebServer(FileSystem *fileSystem, Interfaces::ITopicCallback* actionCallback);
        virtual void loop(unsigned long time);
        virtual void setup();

        virtual ESP8266WebServer* getServer() { return _webServer; };
        virtual Interfaces::ITopicCallback* getCallback() { return _actionCallback; }
    };    
}

#endif  //!_WEBSERVER_H_