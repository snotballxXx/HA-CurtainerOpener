#ifndef _CURTAIN_CONTROL_H
#define _CURTAIN_CONTROL_H
#include <Arduino.h>
#include "../interfaces/IArduinoBase.h"
#include "../control/State.h"

namespace Interfaces
{
    class IMessenger;
}

namespace Control
{
    class DebounceSwitch;

    class MotorDriver : public Interfaces::IAduninoBase
    {
        const int incCount = 1;

        Control::DebounceSwitch *_switch;
        State _currentState;
        State _newState;
        bool _switchTriggered;
        int _stepCount;
        int _pinStep;
        int _pinDir;
        int _pinEnable;
        int _pinStopSwitch;
        String _name;

    public:
        MotorDriver(
            int pinStep,
            int pinDir,
            int pinEnable,
            int pinStopSwitch,
            String name);

        virtual void loop(unsigned long time);
        virtual void setup();
        State getTransistionState() { return _newState; }
        State getState() { return _currentState; }
        void setState(State controlState) { _newState = controlState; };
        void moveCurtain();
    };
}

#endif //_CURTAIN_CONTROL_H