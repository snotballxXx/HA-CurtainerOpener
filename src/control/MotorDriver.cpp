#include "MotorDriver.h"
#include "../interfaces/IMessenger.h"
#include "./Repository.h"
#include "../constants.h"
#include "./DebounceSwitch.h"

using namespace Control;

MotorDriver::MotorDriver(
    int pinStep,
    int pinDir,
    int pinEnable,
    int pinStopSwitch,
    String name) : _currentState(State::Stopped),
                   _newState(State::Stopped),
                   _atHome(false),
                   _stepCount(0),
                   _pinStep(pinStep),
                   _pinDir(pinDir),
                   _pinEnable(pinEnable),
                   _pinStopSwitch(pinStopSwitch),
                   _name(name),
                   _calibratingPriorToMove(false)
{
}

void MotorDriver::setup()
{
    _switch = new DebounceSwitch(_pinStopSwitch, 100L, LOW, _name);

    pinMode(_pinStep, OUTPUT);
    pinMode(_pinDir, OUTPUT);
    pinMode(_pinEnable, OUTPUT);

    digitalWrite(_pinEnable, HIGH);
    digitalWrite(_pinStep, LOW);

    if (!_switch->isTriggered())
    {
        _newState = State::Calibrate;
        Serial.println("Calibration mode active " + _name);
    }
    else
    _atHome = true;
}

void MotorDriver::loop(unsigned long time)
{
    auto switchClosed = _switch->isTriggered();
    if (_atHome)
    {
        Serial.println("Switch Triggered, now open " + _name);
        _stepCount = 0;
        _atHome = false;
        _currentState = _newState = State::Open;
        digitalWrite(_pinEnable, HIGH);
        if (_calibratingPriorToMove)
        {
            _newState = State::Closing;
            _calibratingPriorToMove = false;
        }
    }

    if (((_currentState == State::Closing || _currentState == State::Closed || _currentState == State::Stopped) && _newState == State::Opening) ||
        ((_currentState == State::Opening || _currentState == State::Open || _currentState == State::Stopped) && _newState == State::Closing) ||
        _newState == State::Calibrate)
    {
        if (_newState == State::Closing && !switchClosed)
        {
            _calibratingPriorToMove = true;
            _currentState = State::Calibrate;
            _newState = State::PendingChange;
        }
        else 
        {
            _currentState = _newState;
            _newState = State::PendingChange;
        }
        digitalWrite(_pinEnable, LOW);
    }
    else if ((_currentState == State::Opening && _newState == State::Open) ||
             (_currentState == State::Closing && _newState == State::Closed) ||
             _newState == State::Stopped)
    {
        _currentState = _newState;
        _newState = State::PendingChange;
        digitalWrite(_pinEnable, HIGH);
    }

    if (_currentState == State::Closing)
        moveCurtain();

    if (_currentState == State::Opening || _currentState == State::Calibrate)
        moveCurtain();
}

void MotorDriver::moveCurtain()
{
    auto repo = Repository::getInstance();
    const auto motorOpen = repo->getMotorDirection(_pinDir);
    const auto motorClose = motorOpen == HIGH ? LOW : HIGH;

    digitalWrite(_pinDir, (_currentState == State::Opening || _currentState == State::Calibrate) ? motorOpen : motorClose);

    delayMicroseconds(2);

    Serial.print("Moving, currently at ");
    Serial.print(_stepCount);
    Serial.println(", " + _name);

    for (int i = 0; i <= incCount; i++)
    {
        if (_switch->isTriggered() && (_currentState == State::Opening || _currentState == State::Calibrate))
        {
            _atHome = true;
            return;
        }

        digitalWrite(_pinStep, HIGH);
        delayMicroseconds(1000);
        digitalWrite(_pinStep, LOW);
        delayMicroseconds(1000);
    }

    _stepCount += (((_currentState == State::Opening || _currentState == State::Calibrate) ? -1 : 1) * incCount);

    if (_stepCount >= repo->getCloseStepCount())
    {
        Serial.println("Reached closed position " + _name);
        _newState = State::Closed;
    }

    if (_stepCount < -500 && _currentState != State::Calibrate)
    {
        Serial.println("Soft stop due to negative count " + _name);
        _atHome = true;
    }
}
