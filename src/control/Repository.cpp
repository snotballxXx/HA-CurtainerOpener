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
        _motor1Direction = EEPROM.read(MOTOR1_DIR_ADDR);
        _motor2Direction = EEPROM.read(MOTOR2_DIR_ADDR);
        _closeStepCount = EEPROM.read(STEP_COUNT_ADDR_HIGH) << 8;
        _closeStepCount |= EEPROM.read(STEP_COUNT_ADDR_LOW);      
    }
    else
    {
        Serial.println("Repo default");
        EEPROM.write(MAGIC_NUMBER_ADDR, MAGIC_NUMBER);
        setMotorDirection(MOTOR1_DIR_PIN, DEFAULT_VALUE_MOTOR1_DIR);
        setMotorDirection(MOTOR2_DIR_PIN, DEFAULT_VALUE_MOTOR2_DIR);
        setCloseStepCount(DEFAULT_VALUE_STEP_COUNT);
        EEPROM.commit();            
    }
}

void Repository::setMotorDirection(int id, byte value)
{
    if (id == MOTOR1_DIR_PIN)
    {
        _motor1Direction = value; 
        EEPROM.write(MOTOR1_DIR_ADDR, value);
        EEPROM.commit();
    }
    else
    {
        _motor2Direction = value; 
        EEPROM.write(MOTOR2_DIR_ADDR, value);
        EEPROM.commit();
    }
}  

void Repository::setCloseStepCount(unsigned short value)
{
    _closeStepCount = value;
    EEPROM.write(STEP_COUNT_ADDR_HIGH, value >> 8);
    EEPROM.write(STEP_COUNT_ADDR_LOW, value & 0x00FF);

    EEPROM.commit();         
} 