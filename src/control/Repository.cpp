#include <arduino.h>
#include <EEPROM.h>
#include "Repository.h"
#include "Constants.h"

using namespace Control;

Repository::Repository()
{
    EEPROM.begin(512);
    
    byte magicNumber = EEPROM.read(MAGIC_NUMBER_ADDR);
    if (magicNumber == MAGIC_NUMBER)
    {
        Serial.println("Repo loading");
        _motorDirection = EEPROM.read(MOTOR1_DIR_ADDR);
        _closeStepCount = EEPROM.read(STEP_COUNT_ADDR_HIGH) << 8;
        _closeStepCount |= EEPROM.read(STEP_COUNT_ADDR_LOW);      
    }
    else
    {
        Serial.println("Repo default");
        EEPROM.write(MAGIC_NUMBER_ADDR, MAGIC_NUMBER);
        setMotorDirection(DEFAULT_VALUE_MOTOR1_DIR);
        setCloseStepCount(DEFAULT_VALUE_STEP_COUNT);
        EEPROM.commit();            
    }
}

void Repository::setMotorDirection(byte value)
{ 
    _motorDirection = value; 
    EEPROM.write(MOTOR1_DIR_ADDR, value);
    EEPROM.commit();
}

void Repository::setCloseStepCount(unsigned short value)
{
    _closeStepCount = value;
    EEPROM.write(STEP_COUNT_ADDR_HIGH, value >> 8);
    EEPROM.write(STEP_COUNT_ADDR_LOW, value & 0x00FF);

    EEPROM.commit();         
} 