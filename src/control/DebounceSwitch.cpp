#include <Arduino.h>
#include "./DebounceSwitch.h"

using namespace Control;

//DebounceSwitch* DebounceSwitch::instance;

DebounceSwitch::DebounceSwitch(int pin, unsigned long debounceDelay, int mode, String name) : 
_inputPin(pin),
_debounceDelay(debounceDelay),
_lastDebounceTime(0),
_changeState(LOW),
_currentState(LOW),
_mode(mode),
_name(name)
{
    pinMode(_inputPin, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(_inputPin), DebounceSwitch::debounceISR, CHANGE);

    //DebounceSwitch::instance = this;

    _currentState = digitalRead(_inputPin);
}

// void IRAM_ATTR DebounceSwitch::debounceISR() 
// {
//     instance->_lastDebounceTime = millis();
//     instance->_changeState = digitalRead(instance->_inputPin);
// }

bool DebounceSwitch::isTriggered()
{
    unsigned long time = millis();
    auto state = digitalRead(_inputPin);
    if (((_lastDebounceTime + _debounceDelay) < time) && /*_changeState == state &&*/ _currentState != state)
    {
        _lastDebounceTime = time;
        _currentState = state;
        Serial.print("Switch triggered on " + _name + " ");
        Serial.println(_currentState == _mode);
    }

    return _currentState == _mode;
}

void DebounceSwitch::loop(unsigned long time)
{
}

void DebounceSwitch::setup() {}