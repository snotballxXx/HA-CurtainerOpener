#include "CurtainControl.h"
#include "../interfaces/IMessenger.h"
#include "./Repository.h"
#include "../constants.h"

using namespace Control;

#define MOTOR1_STEP_PIN      D2
#define MOTOR1_DIR_PIN       D3
#define MOTOR1_ENABLE_PIN    D4
#define MOTOR2_STEP_PIN      D5
#define MOTOR2_DIR_PIN       D6
#define MOTOR2_ENABLE_PIN    D7
#define END_STOP_SWITCH      D8

#define STATE_OPEN           "OPEN"
#define STATE_OPENING        "OPENING"
#define STATE_CLOSED         "CLOSED"
#define STATE_CLOSING        "CLOSING"
#define STATE_STOPPED        "STOPPED"
#define CMD_OPEN             "OPEN"
#define CMD_CLOSE            "CLOSE"
#define CMD_STOP             "STOP"

int stepCount = 0;
const int incCount = 100;
bool hardStop = false;

CurtainControl::CurtainControl(Interfaces::IMessenger* messenger) :
_messenger(messenger),
_currentState(State::Stopped),
_newState(State::Stopped)
{
}

void IRAM_ATTR limitSwitchTrigger()
{
    digitalWrite(MOTOR1_ENABLE_PIN, HIGH);
    digitalWrite(MOTOR2_ENABLE_PIN, HIGH);
    hardStop = true;
}

void CurtainControl::setup()
{
    _messenger->subscribe(COMMAND_TOPIC, this);

    pinMode(MOTOR1_STEP_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR1_ENABLE_PIN, OUTPUT);

    pinMode(MOTOR2_STEP_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_ENABLE_PIN, OUTPUT);

    pinMode(END_STOP_SWITCH, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(END_STOP_SWITCH), limitSwitchTrigger, FALLING);

    digitalWrite(MOTOR1_ENABLE_PIN, HIGH);
    digitalWrite(MOTOR1_STEP_PIN, LOW);

    digitalWrite(MOTOR2_ENABLE_PIN, HIGH);
    digitalWrite(MOTOR2_STEP_PIN, LOW);

    auto limitSwitch = digitalRead(END_STOP_SWITCH);
    if (limitSwitch == HIGH)
        _newState = State::Opening;
    else
        _newState = State::Open;
} 

void CurtainControl::loop(unsigned long time)
{
    if (hardStop)
    {
        stepCount = 0;
        hardStop = false;
        _newState = State::Open;
    }

    if (((_currentState == State::Closing || _currentState == State::Closed || _currentState == State::Stopped) && _newState == State::Opening) ||
        ((_currentState == State::Opening || _currentState == State::Open || _currentState == State::Stopped) && _newState == State::Closing))
    {
        _currentState = _newState;
        digitalWrite(MOTOR1_ENABLE_PIN, LOW);
        digitalWrite(MOTOR2_ENABLE_PIN, LOW);
        sendStateUpdate();     
    }
    else if ((_currentState == State::Opening && _newState == State::Open) || 
             (_currentState == State::Closing && _newState == State::Closed) ||
             _newState == State::Stopped)
    {
        _currentState = _newState;
        digitalWrite(MOTOR1_ENABLE_PIN, HIGH);
        digitalWrite(MOTOR2_ENABLE_PIN, HIGH);
        sendStateUpdate();     
    }

    if (_currentState == State::Closing)
        moveCurtain();
    
    if (_currentState == State::Opening)
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
        auto repo = Repository::getInstance();
        if (stepCount < repo->getCloseStepCount())
            _newState = State::Closing;
    }
}

void CurtainControl::sendStateUpdate()
{
    switch (_currentState)
    {
        case State::Closed: _messenger->sendMessage(STATE_TOPIC, STATE_CLOSED);break;
        case State::Closing: _messenger->sendMessage(STATE_TOPIC, STATE_CLOSING);break;
        case State::Open: _messenger->sendMessage(STATE_TOPIC, STATE_OPEN);break;
        case State::Opening: _messenger->sendMessage(STATE_TOPIC, STATE_OPENING);break;
        case State::Stopped: _messenger->sendMessage(STATE_TOPIC, STATE_STOPPED);break;
    }

    Serial.println("State change to " + _currentState);
}

void CurtainControl::moveCurtain()
{
    auto repo = Repository::getInstance();
    const auto m1Open = repo->getMotor1Direction();
    const auto m2Open = repo->getMotor2Direction();
    const auto m1Close = m1Open == HIGH ? LOW : HIGH;
    const auto m2Close = m2Open == HIGH ? LOW : HIGH;

    digitalWrite(_currentState == State::Opening ? m1Open : m1Close, LOW);
    digitalWrite(_currentState == State::Opening ? m2Open : m2Close, LOW);
    delayMicroseconds(2);

    for(int i=0; i <= incCount; i++) 
    {
        if (hardStop)
            return;
        digitalWrite(MOTOR1_STEP_PIN, HIGH);
        digitalWrite(MOTOR2_STEP_PIN, HIGH);
        delayMicroseconds(1100);
        digitalWrite(MOTOR1_STEP_PIN, LOW);
        digitalWrite(MOTOR2_STEP_PIN, LOW);
        delayMicroseconds(2);
    }
    stepCount += ((_currentState == State::Opening ? -1 : 1) * incCount); 
    delayMicroseconds(2);

    if (stepCount >= repo->getCloseStepCount())
        _newState = State::Closed;
}
