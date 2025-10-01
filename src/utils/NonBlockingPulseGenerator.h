#ifndef _NONBLOCKING_PULSE_GENERATOR
#define _NONBLOCKING_PULSE_GENERATOR
#include <Arduino.h>

namespace Utils
{
    class NonBlockingPulseGenerator
    {
        int      _pin;
        uint64_t _onDuration;
        uint64_t _offDuration;
        bool     _active;
        uint64_t _offTime;
        uint64_t _offActiveTime;

       public:
        NonBlockingPulseGenerator(int outputPin, uint64_t microsOn, uint64_t microsOff);

        void triggerPulse();

        bool pulseActive();
    };
}  // namespace Utils

#endif  //_NONBLOCKING_PULSE_GENERATOR