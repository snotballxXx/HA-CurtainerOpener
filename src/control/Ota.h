#ifndef _IR_OTA_H_
#define _IR_OTA_H_

#include "../interfaces/IArduinoBase.h"

namespace Interfaces
{
    class IWebServer;
}

namespace Control
{
    class Ota : public Interfaces::IAduninoBase
    {
        Interfaces::IWebServer* _webserver;

       public:
        Ota(Interfaces::IWebServer* webserver);
        virtual void loop(unsigned long time);
        virtual void setup();
    };
}  // namespace Control

#endif  //!_IR_OTA_H_