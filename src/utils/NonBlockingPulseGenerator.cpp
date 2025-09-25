
#include "./NonBlockingPulseGenerator.h"
#include "./control/WebServer.h"

using namespace Utils;

extern Control::WebServer *webServer;

NonBlockingPulseGenerator::NonBlockingPulseGenerator(int outputPin, uint64_t microsOn, uint64_t microsOff) : _pin(outputPin),
                                                                                                             _onDuration(microsOn),
                                                                                                             _offDuration(microsOff),
                                                                                                             _active(false),
                                                                                                             _offTime(0),
                                                                                                             _offActiveTime(0)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void NonBlockingPulseGenerator::triggerPulse()
{
    if (!_active)
    {
        digitalWrite(_pin, HIGH);
        auto startTime = micros64();
        _offTime = startTime + _onDuration;
        _offActiveTime = startTime + _onDuration + (webServer->getSlider() * _offDuration);
        //_offActiveTime = startTime + _onDuration + _offDuration;
        _active = true;
    }
}

bool NonBlockingPulseGenerator::pulseActive()
{
    if (_active)
    {
        auto time = micros64();
        if (time > _offActiveTime)
        {
            digitalWrite(_pin, LOW);
            _active = false;
        }
        else if (time > _offTime)
            digitalWrite(_pin, LOW);
    }

    return _active;
}