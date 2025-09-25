#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "../interfaces/IArduinoBase.h"
#include "../interfaces/ITopicCallback.h"

#include "./State.h"

namespace Interfaces
{
    class IMessenger;
    class IMotor;
}

namespace Control
{
    class MotorDriver;

    class Controller : public Interfaces::IAduninoBase, public Interfaces::ITopicCallback
    {
        Interfaces::IMotor *_motor1;
        Interfaces::IMotor *_motor2;
        Interfaces::IMessenger *_messenger;
        State _currentState;
        bool _pendingStateUpdate;
    public:
        Controller(Interfaces::IMessenger *messenger);

        virtual void loop(unsigned long time);
        virtual void setup();
        virtual void messageReceived(const String &topic, const String &payload);
        void sendStateUpdate();
    };
}

#endif // _CONTROLLER_H