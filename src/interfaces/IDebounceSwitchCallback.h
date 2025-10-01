#ifndef _IDEBOUNCE_SWITCH_CALLBACK_H
#define _IDEBOUNCE_SWITCH_CALLBACK_H

namespace Interfaces
{
    class IDebounceSwitchCallback
    {
       public:
        virtual void switchTrigger(bool state) = 0;
    };
}  // namespace Interfaces

#endif