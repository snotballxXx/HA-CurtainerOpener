#include <Arduino.h>
#include "./DebounceSwitch.h"
#include <interfaces/ILogger.h>

using namespace Control;

extern Interfaces::ILogger* logger;

DebounceSwitch::DebounceSwitch(int pin, unsigned long debounceDelay, int mode, String name)
    : _inputPin(pin),
      _debounceDelay(debounceDelay),
      _lastDebounceTime(0),
      _changeState(LOW),
      _currentState(LOW),
      _mode(mode),
      _name(name)
{
    pinMode(_inputPin, INPUT_PULLUP);

    _currentState = digitalRead(_inputPin);
}

bool DebounceSwitch::isTriggered()
{
    unsigned long time  = millis();
    auto          state = digitalRead(_inputPin);
    if (((_lastDebounceTime + _debounceDelay) < time) && _currentState != state)
    {
        _lastDebounceTime = time;
        _currentState     = state;
        logger->sendLog("Switch triggered on " + _name + " now " + (_currentState == _mode ? "Closed" : "Open"));
    }

    return _currentState == _mode;
}

void DebounceSwitch::loop(unsigned long time) {}

void DebounceSwitch::setup() {}