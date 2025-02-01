#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "../interfaces/IWebServer.h"
#include <ESP8266WebServer.h>

namespace Control
{
    class WebServer : public Interfaces::IWebServer
    {
        ESP8266WebServer* _webServer;
        public:
        WebServer();
        virtual void loop(unsigned long time);
        virtual void setup();

        virtual ESP8266WebServer* getServer() { return _webServer; };
    };    
}

#endif  //!_WEBSERVER_H_