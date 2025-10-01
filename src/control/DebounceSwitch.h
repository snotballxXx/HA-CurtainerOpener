#ifndef _DEBOUNCE_SWITCH_H
#define _DEBOUNCE_SWITCH_H

#include "../interfaces/IArduinoBase.h"

namespace Control
{
    class DebounceSwitch : public Interfaces::IAduninoBase
    {
       private:
        int           _inputPin;
        unsigned long _debounceDelay;
        unsigned long _lastDebounceTime;
        int           _changeState;
        int           _currentState;
        int           _mode;
        String        _name;

        //       static DebounceSwitch* instance;
        //       static void debounceISR();
       public:
        DebounceSwitch(int pin, unsigned long debounceDelay, int mode, String name);
        virtual void loop(unsigned long time);
        virtual void setup();
        bool         isTriggered();
    };
}  // namespace Control

#endif  //_DEBOUNCE_SWITCH_H