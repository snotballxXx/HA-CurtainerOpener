#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "../interfaces/IWebServer.h"
#include <ESP8266WebServer.h>
#include "./FileSystem.h"

namespace Control
{
    class WebServer : public Interfaces::IWebServer
    {
        ESP8266WebServer* _webServer;
        FileSystem *_fileSystem;

        public:
        WebServer(FileSystem *fileSystem);
        virtual void loop(unsigned long time);
        virtual void setup();

        virtual ESP8266WebServer* getServer() { return _webServer; };
    };    
}

#endif  //!_WEBSERVER_H_