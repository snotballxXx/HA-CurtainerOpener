#ifndef _ICURTAIN_CONTROL_H_
#define _ICURTAIN_CONTROL_H_

namespace Interfaces
{
    class ICurtainControl
    {
        public:
        virtual void calibrate() = 0;
    };
}

#endif  //!_ICURTAIN_CONTROL_H_