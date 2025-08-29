#ifndef _NONBLOCKING_PULSE_GENERATOR
#define _NONBLOCKING_PULSE_GENERATOR
#include <Arduino.h>

namespace Utils
{
    class NonBlockingPulseGenerator
    {
        uint64_t _onDuration;
        uint64_t _offDuration;
        uint64_t _offTime;
        uint64_t _offActiveTime;
        bool _active;
        int _pin;

    public:
        NonBlockingPulseGenerator(int outputPin, uint64_t microsOn, uint64_t microsOff);

        void trigger();

        bool update();
    };
}

#endif //_NONBLOCKING_PULSE_GENERATOR