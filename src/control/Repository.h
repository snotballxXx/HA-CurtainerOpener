#include <constants.h>
#ifndef __REPOSITORY__H__
#define __REPOSITORY__H__

namespace Control
{
    class Repository
    {
        byte _motor1Direction;
        byte _motor2Direction;
        unsigned short _closeStepCount;
        Repository();
        
        public:
        static Repository* getInstance() {
            static Repository instance;
            return &instance;
        }

        byte getMotorDirection(int id) { return id == MOTOR1_DIR_PIN ? _motor1Direction : _motor2Direction; }
        void setMotorDirection(int id, byte value);

        unsigned short getCloseStepCount() { return _closeStepCount; }
        void setCloseStepCount(unsigned short value);
    };
}
#endif  //!__REPOSITORY__H__