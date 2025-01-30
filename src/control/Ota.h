#ifndef _IR_OTA_H_
#define _IR_OTA_H_

#include "../interfaces/IArduinoBase.h"

namespace Control
{
    class Ota : public Interfaces::IAduninoBase
    {
        public:
        Ota();
        virtual void loop(unsigned long time);
        virtual void setup();        
    };    
}

#endif  //!_IR_OTA_H_