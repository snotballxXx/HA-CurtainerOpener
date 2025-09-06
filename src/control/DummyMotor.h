#ifndef _DUMMY_MOTOR_H
#define _DUMMY_MOTOR_H
#include <Arduino.h>
#include "../interfaces/IMotor.h"
#include "../control/State.h"

namespace Control
{
    class DummyMotor : public Interfaces::IMotor
    {
        Interfaces::IMotor* _proxyMotor;

    public:
        DummyMotor(Interfaces::IMotor* proxyMotor)
        {
            _proxyMotor = proxyMotor;
        }

        virtual void loop(unsigned long time) {};
        virtual void setup() {};
        virtual State getTransistionState() { return _proxyMotor->getTransistionState(); }
        virtual State getState() { return _proxyMotor->getState(); }
        virtual void setState(State controlState) { };
    };
}

#endif //_DUMMY_MOTOR_H