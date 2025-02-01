#ifndef _DEBOUNCE_SWITCH_H
#define _DEBOUNCE_SWITCH_H


namespace Control
{
    class DebounceSwitch
    {
        private:
            int _inputPin;
            unsigned long _debounceDelay;
            unsigned long _lastDebounceTime;
            int _changeState;
            int _currentState;
            int _mode;

            static DebounceSwitch* instance;
            static void debounceISR();
        public:
            DebounceSwitch(int pin, unsigned long debounceDelay, int mode);

            bool isTriggered();
    };
}

#endif //_DEBOUNCE_SWITCH_H