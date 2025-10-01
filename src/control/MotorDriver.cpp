#include "MotorDriver.h"
#include "../interfaces/IMessenger.h"
#include "./Repository.h"
#include "../constants.h"
#include "./DebounceSwitch.h"
#include "../utils/NonBlockingPulseGenerator.h"
#include "../utils/Helpers.h"
#include <interfaces/ILogger.h>

using namespace Control;

extern Interfaces::ILogger* logger;

MotorDriver::MotorDriver(int pinStep, int pinDir, int pinEnable, int pinStopSwitch, String name)
    : _currentState(State::Stopped),
      _newState(State::Stopped),
      _arrivedHome(false),
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
    //_pulseGenerator = new Utils::NonBlockingPulseGenerator(_pinStep, 1500, 50000);
    _pulseGenerator = new Utils::NonBlockingPulseGenerator(_pinStep, 1500, 1000);
    _switch         = new DebounceSwitch(_pinStopSwitch, 100L, LOW, _name);

    pinMode(_pinDir, OUTPUT);
    pinMode(_pinEnable, OUTPUT);
    digitalWrite(_pinEnable, HIGH);

    if (!_switch->isTriggered())
    {
        _newState = State::Calibrate;
        logger->sendLog("Calibration mode active " + _name);
    }
    else
        _arrivedHome = true;
}

void MotorDriver::loop(unsigned long time)
{
    auto pulseComplete = !_pulseGenerator->pulseActive();

    auto switchClosed = _switch->isTriggered();
    if (_arrivedHome)
    {
        _stepCount    = 0;
        _arrivedHome  = false;
        _currentState = _newState = State::Closed;
        digitalWrite(_pinEnable, HIGH);
        if (_calibratingPriorToMove)
        {
            _newState               = State::Opening;
            _calibratingPriorToMove = false;
            logger->sendLog("Calibration prior to move complete, now opening");
        }
    }

    if (((_currentState == State::Closing || _currentState == State::Closed || _currentState == State::Stopped) &&
         _newState == State::Opening) ||
        ((_currentState == State::Opening || _currentState == State::Open || _currentState == State::Stopped) &&
         _newState == State::Closing) ||
        _newState == State::Calibrate)
    {
        if (_newState == State::Opening && !switchClosed)
        {
            _calibratingPriorToMove = true;
            _currentState           = State::Calibrate;
            _newState               = State::PendingChange;
            logger->sendLog("Open request, switch is open so going to calibrate closing prior to opening");
        }
        else
        {
            _currentState = _newState;
            _newState     = State::PendingChange;
            logger->sendLog("Initiating state change " + Utils::Helpers::stateToString(_currentState));
        }
        digitalWrite(_pinEnable, LOW);
    }
    else if ((_currentState == State::Opening && _newState == State::Open) ||
             (_currentState == State::Closing && _newState == State::Closed) || _newState == State::Stopped)
    {
        _currentState = _newState;
        _newState     = State::PendingChange;
        digitalWrite(_pinEnable, HIGH);
    }

    if ((_currentState == State::Closing || _currentState == State::Calibrate || _currentState == State::Opening) &&
        pulseComplete)
        moveCurtain();
}

void MotorDriver::moveCurtain()
{
    auto       repo       = Repository::getInstance();
    const auto motorClose = repo->getMotorDirection(_pinDir);
    const auto motorOpen  = motorClose == HIGH ? LOW : HIGH;

    digitalWrite(_pinDir,
                 (_currentState == State::Closing || _currentState == State::Calibrate) ? motorClose : motorOpen);

    if (_switch->isTriggered() && (_currentState == State::Closing || _currentState == State::Calibrate))
    {
        _arrivedHome = true;
        return;
    }

    _pulseGenerator->triggerPulse();

    _stepCount += (((_currentState == State::Closing || _currentState == State::Calibrate) ? -1 : 1) * incCount);

    if (_stepCount % 500 == 0) logger->sendLog("Moving " + _name + " " + _stepCount);

    if (_stepCount >= repo->getMaxStepCount())
    {
        logger->sendLog("Reached open position " + _name);
        _newState = State::Open;
    }

    if (_stepCount < -500 && _currentState != State::Calibrate)
    {
        logger->sendLog("Soft stop due to negative count" + _name);
        _arrivedHome = true;
    }
}
