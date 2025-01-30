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

        byte getMotor1Direction() { return _motor1Direction; }
        void setMotor1Direction(byte value);

        byte getMotor2Direction() { return _motor2Direction; }
        void setMotor2Direction(byte value);    

        unsigned short getCloseStepCount() { return _closeStepCount; }
        void setCloseStepCount(unsigned short value);
    };
}
#endif  //!__REPOSITORY__H__