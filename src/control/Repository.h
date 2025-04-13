#ifndef __REPOSITORY__H__
#define __REPOSITORY__H__

namespace Control
{
    class Repository
    {
        byte _motorDirection;
        unsigned short _closeStepCount;
        Repository();
        
        public:
        static Repository* getInstance() {
            static Repository instance;
            return &instance;
        }

        byte getMotorDirection() { return _motorDirection; }
        void setMotorDirection(byte value);  

        unsigned short getCloseStepCount() { return _closeStepCount; }
        void setCloseStepCount(unsigned short value);
    };
}
#endif  //!__REPOSITORY__H__