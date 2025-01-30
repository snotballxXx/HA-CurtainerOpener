#ifndef _WIFI_H_
#define _WIFI_H_
#include "../interfaces/IWifi.h"

namespace Control
{
    class Wifi : public Interfaces::IWifi
    {
        public:
        virtual void loop(unsigned long time);
        virtual void setup();
    };
}

#endif  //!_WIFI_H_