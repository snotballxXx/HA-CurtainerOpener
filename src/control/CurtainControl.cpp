#include "CurtainControl.h"
#include "../interfaces/IMessenger.h"
#include "./Repository.h"
#include "../constants.h"
#include "./DebounceSwitch.h"

using namespace Control;

CurtainControl::CurtainControl(Interfaces::IMessenger* messenger) :
_messenger(messenger),
_currentState(State::Stopped),
_newState(State::Stopped),
_switchTriggered(false),
_stepCount(0)
{
}

void CurtainControl::setup()
{
    _switch = new DebounceSwitch(END_STOP_SWITCH, 100L, HIGH);
    _messenger->subscribe(COMMAND_TOPIC, this);

    pinMode(MOTOR1_STEP_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR1_ENABLE_PIN, OUTPUT);

    pinMode(MOTOR2_STEP_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_ENABLE_PIN, OUTPUT);

    digitalWrite(MOTOR1_ENABLE_PIN, HIGH);
    digitalWrite(MOTOR1_STEP_PIN, LOW);

    digitalWrite(MOTOR2_ENABLE_PIN, HIGH);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    if (!_switch->isTriggered())
    {
        _newState = State::Calibrate;
        Serial.println("Calibration mode active");
    }
    else
        _switchTriggered = true;
} 

void CurtainControl::loop(unsigned long time)
{
    _switch->isTriggered();
    if (_switchTriggered)
    {
        Serial.println("Switch Triggered, now open");
        _stepCount = 0;
        _switchTriggered = false;
        _currentState = _newState = State::Open;
        sendStateUpdate();
    }

    if (((_currentState == State::Closing || _currentState == State::Closed || _currentState == State::Stopped) && _newState == State::Opening) ||
        ((_currentState == State::Opening || _currentState == State::Open || _currentState == State::Stopped) && _newState == State::Closing) ||
        _newState == State::Calibrate)
    {
        _currentState = _newState;
        _newState = State::PendingChange;
        digitalWrite(MOTOR1_ENABLE_PIN, LOW);
        digitalWrite(MOTOR2_ENABLE_PIN, LOW);
        sendStateUpdate();     
    }
    else if ((_currentState == State::Opening && _newState == State::Open) || 
             (_currentState == State::Closing && _newState == State::Closed) ||
             _newState == State::Stopped)
    {
        _currentState = _newState;
        _newState = State::PendingChange;
        digitalWrite(MOTOR1_ENABLE_PIN, HIGH);
        digitalWrite(MOTOR2_ENABLE_PIN, HIGH);
        sendStateUpdate();     
    }

    if (_currentState == State::Closing)
        moveCurtain();
    
    if (_currentState == State::Opening || _currentState == State::Calibrate)
        moveCurtain();
}

void CurtainControl::messageReceived(const String& topic, const String& payload)
{   
    if (String(CMD_OPEN) == payload)
    {
        Serial.println("Request to open");
        _newState = State::Opening;
    } 
    else if (String(CMD_STOP) == payload)
    {
        Serial.println("Request to stop");
        _newState = State::Stopped;
    }
    else if (String(CMD_CLOSE) == payload)
    {
        Serial.println("Request to close");
        _newState = State::Closing;
    }
}

void CurtainControl::sendStateUpdate()
{
    String msg("");
    switch (_currentState)
    {
        case State::Closed: _messenger->sendMessage(STATE_TOPIC, STATE_CLOSED);msg = "STATE_CLOSED";break;
        case State::Closing: _messenger->sendMessage(STATE_TOPIC, STATE_CLOSING);msg = "STATE_CLOSING";break;
        case State::Open: _messenger->sendMessage(STATE_TOPIC, STATE_OPEN);msg = "STATE_OPEN";break;
        case State::Opening: _messenger->sendMessage(STATE_TOPIC, STATE_OPENING);msg = "STATE_OPENING";break;
        case State::Stopped: _messenger->sendMessage(STATE_TOPIC, STATE_STOPPED);msg = "STATE_STOPPED";break;
        case State::PendingChange: break;
        case State::Calibrate: msg = "Calibrate";break;
    }

    Serial.println("State change to " + msg);
}

void CurtainControl::moveCurtain()
{
    auto repo = Repository::getInstance();
    const auto m1Open = repo->getMotor1Direction();
    const auto m2Open = repo->getMotor2Direction();
    const auto m1Close = m1Open == HIGH ? LOW : HIGH;
    const auto m2Close = m2Open == HIGH ? LOW : HIGH;

    digitalWrite(MOTOR1_DIR_PIN, (_currentState == State::Opening || _currentState == State::Calibrate) ? m1Open : m1Close);
    digitalWrite(MOTOR2_DIR_PIN, (_currentState == State::Opening || _currentState == State::Calibrate) ? m2Open : m2Close);

    delayMicroseconds(2);

    Serial.print("Moving, currently at ");
    Serial.println(_stepCount);

    for(int i=0; i <= incCount; i++) 
    {
        if (_switch->isTriggered() && (_currentState == State::Opening || _currentState == State::Calibrate))
        {
            _switchTriggered = true;
            return;
        }

        digitalWrite(MOTOR1_STEP_PIN, HIGH);
        digitalWrite(MOTOR2_STEP_PIN, HIGH);
        delayMicroseconds(1100);
        digitalWrite(MOTOR1_STEP_PIN, LOW);
        digitalWrite(MOTOR2_STEP_PIN, LOW);
        delayMicroseconds(2);
    }

    _stepCount += (((_currentState == State::Opening || _currentState == State::Calibrate) ? -1 : 1) * incCount); 
    delayMicroseconds(2);

    if (_stepCount >= repo->getCloseStepCount())
    {
        Serial.println("Reached closed position");
        _newState = State::Closed;
    }

    if (_stepCount < -500 && _currentState != State::Calibrate)
    {
        Serial.println("Soft stop due to negative count");
        _switchTriggered = true;
    }
}
