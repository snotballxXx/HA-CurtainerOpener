#ifndef _CURTAIN_CONTROL_H
#define _CURTAIN_CONTROL_H
#include <Arduino.h>
#include "../interfaces/IMotor.h"
#include "../control/State.h"

namespace Interfaces
{
    class IMessenger;
}

namespace Utils
{
    class NonBlockingPulseGenerator;
}

namespace Control
{
    class DebounceSwitch;

    class MotorDriver : public Interfaces::IMotor
    {
        const int incCount = 1;

        Utils::NonBlockingPulseGenerator* _pulseGenerator;
        Control::DebounceSwitch*          _switch;
        State                             _currentState;
        State                             _newState;
        bool                              _arrivedHome;
        int                               _stepCount;
        int                               _pinStep;
        int                               _pinDir;
        int                               _pinEnable;
        int                               _pinStopSwitch;
        String                            _name;
        bool                              _calibratingPriorToMove;

        void moveCurtain();

       public:
        MotorDriver(int pinStep, int pinDir, int pinEnable, int pinStopSwitch, String name);

        virtual void  loop(unsigned long time);
        virtual void  setup();
        virtual State getTransistionState() { return _newState; }
        virtual State getState() { return _currentState; }
        virtual void  setState(State controlState) { _newState = controlState; };
    };
}  // namespace Control

#endif  //_CURTAIN_CONTROL_H