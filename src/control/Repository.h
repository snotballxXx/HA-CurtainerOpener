#include <constants.h>
#ifndef __REPOSITORY__H__
#define __REPOSITORY__H__
#include "./State.h"
namespace Control
{
    class Repository
    {
        byte _motor1Direction;
        byte _motor2Direction;
        unsigned short _maxStepCount;
        State _initialState;
        String _entityId;
        Repository();
        
        public:
        static Repository* getInstance() {
            static Repository instance;
            return &instance;
        }

        byte getMotorDirection(int id) { return id == MOTOR1_DIR_PIN ? _motor1Direction : _motor2Direction; }
        void setMotorDirection(int id, byte value);

        unsigned short getMaxStepCount() { return _maxStepCount; }
        void setMaxStepCount(unsigned short value);

        State getInitalState() { return _initialState; }
        void setState(State state);

        String getEntityId() { return _entityId; }
        void setEntityId(String id);
    };
}
#endif  //!__REPOSITORY__H__