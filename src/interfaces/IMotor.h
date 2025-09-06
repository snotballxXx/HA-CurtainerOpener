#ifndef _IMOTOR_H_
#define _IMOTOR_H_

#include "./IArduinoBase.h"
#include "./control/State.h"

namespace Interfaces
{
    class IMotor : public IAduninoBase
    {
        public:
        virtual void loop(unsigned long time) = 0;
        virtual void setup() = 0;
        virtual Control::State getTransistionState() = 0;
        virtual Control::State getState() = 0;
        virtual void setState(Control::State controlState) = 0;
    };
}

#endif  //!_IMOTOR_H_