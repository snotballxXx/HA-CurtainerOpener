#ifndef _CURTAIN_CONTROL_H
#define _CURTAIN_CONTROL_H

#include "../interfaces/IArduinoBase.h"
#include "../interfaces/ITopicCallback.h"

namespace Interfaces
{
    class IMessenger;
}

enum State 
{
    Open,
    Opening,
    Closing,
    Closed,
    Stopped
};

namespace Control
{
    class CurtainControl : public Interfaces::IAduninoBase, public Interfaces::ITopicCallback
    {
        Interfaces::IMessenger* _messenger;
        State _currentState;
        State _newState;

        public:
        CurtainControl(Interfaces::IMessenger* messenger);
        virtual void loop(unsigned long time);
        virtual void setup();

        virtual void messageReceived(const String& topic, const String& payload); 
        void moveCurtain();
        void sendStateUpdate();
    };
}

#endif //_CURTAIN_CONTROL_H