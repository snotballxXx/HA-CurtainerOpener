#ifndef _IARDUINOBASE_H_
#define _IARDUINOBASE_H_

namespace Interfaces
{
    class IAduninoBase
    {
        public:
        virtual void loop(unsigned long time) = 0;
        virtual void setup() = 0;
    };
}

#endif  //!_IARDUINOBASE_H_