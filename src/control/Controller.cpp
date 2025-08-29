#include "./Controller.h"
#include "./MotorDriver.h"
#include "../interfaces/IMessenger.h"
#include <constants.h>
#include "./Repository.h"

using namespace Control;

Controller::Controller(Interfaces::IMessenger *messenger) : _messenger(messenger),
                                                            _currentState(State::Stopped),
                                                            _pendingStateUpdate(true)
{
    _motor1 = new MotorDriver(MOTOR1_STEP_PIN, MOTOR1_DIR_PIN, MOTOR1_ENABLE_PIN, END_STOP_SWITCH1, "Motor1");
    _motor2 = new MotorDriver(MOTOR2_STEP_PIN, MOTOR2_DIR_PIN, MOTOR2_ENABLE_PIN, END_STOP_SWITCH2, "Motor2");
}

void Controller::setup()
{
    _messenger->subscribe(COMMAND_TOPIC, this);
    _motor1->setup();
    _motor2->setup();

    if (_motor1->getTransistionState() == Calibrate || _motor2->getTransistionState() == Calibrate)
        _currentState = Calibrate;
}

void Controller::loop(unsigned long time)
{
    _motor1->loop(time);
    _motor2->loop(time);
    State motor1State = _motor1->getState();
    State motor2State = _motor2->getState();

    if (_currentState == State::Calibrate && motor1State != State::Calibrate && motor2State != State::Calibrate)
    {
        if (motor1State == motor2State)
        {
            _currentState = motor1State;
            sendStateUpdate();
        }
        return;
    }

    if ((_currentState == State::Closing && motor1State == State::Closed && motor2State == State::Closed) ||
        (_currentState == State::Opening && motor1State == State::Open && motor2State == State::Open) ||
        (_currentState == State::Stopped && motor1State == State::Stopped && motor2State == State::Stopped))
    {
        _currentState = motor1State;
        sendStateUpdate();
    }
}

void Controller::messageReceived(const String &topic, const String &payload)
{
    _pendingStateUpdate = true;
    if (String(CMD_OPEN) == payload)
    {
        Serial.println("Request to open");
        _currentState = State::Opening;
        sendStateUpdate();
    }
    else if (String(CMD_STOP) == payload)
    {
        Serial.println("Request to stop");
        _currentState = State::Stopped;
        sendStateUpdate();
    }
    else if (String(CMD_CLOSE) == payload)
    {
        Serial.println("Request to close");
        _currentState = State::Closing;
        sendStateUpdate();
    }

    Repository::getInstance()->setState(_currentState);
    _motor1->setState(_currentState);
    _motor2->setState(_currentState);
}

void Controller::sendStateUpdate()
{
    if (_pendingStateUpdate)
    {
        String msg("");
        _pendingStateUpdate = false;
        Repository::getInstance()->setState(_currentState);
        switch (_currentState)
        {
        case State::Closed:
            _messenger->sendMessage(STATE_TOPIC, STATE_CLOSED);
            msg = "STATE_CLOSED";
            break;
        case State::Closing:
            _messenger->sendMessage(STATE_TOPIC, STATE_CLOSING);
            msg = "STATE_CLOSING";
            _pendingStateUpdate = true;
            break;
        case State::Open:
            _messenger->sendMessage(STATE_TOPIC, STATE_OPEN);
            msg = "STATE_OPEN";
            break;
        case State::Opening:
            _messenger->sendMessage(STATE_TOPIC, STATE_OPENING);
            msg = "STATE_OPENING";
            _pendingStateUpdate = true;
            break;
        case State::Stopped:
            _messenger->sendMessage(STATE_TOPIC, STATE_STOPPED);
            msg = "STATE_STOPPED";
            break;
        case State::PendingChange:
            break;
        case State::Calibrate:
            msg = "Calibrate";
            break;
        }

        Serial.println("State change to " + msg);
    }
}