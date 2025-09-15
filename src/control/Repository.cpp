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
        _maxStepCount = EEPROM.read(STEP_COUNT_ADDR_HIGH) << 8;
        _maxStepCount |= EEPROM.read(STEP_COUNT_ADDR_LOW);
        _initialState = (State)EEPROM.read(STATE_ADDR);
        _numberOfMotors = EEPROM.read(NUMBER_OF_MOTORS_ADDR);
        auto entityIdCount = EEPROM.read(ENTITY_ID_COUNT_ADDR);
        _entityId = "";
        for (uint8_t i = 0; i < entityIdCount; ++i)
            _entityId += (char)EEPROM.read(ENTITY_ID_ADDR + i);
    }
    else
    {
        Serial.println("Repo default");
        EEPROM.write(MAGIC_NUMBER_ADDR, MAGIC_NUMBER);
        setMotorDirection(MOTOR1_DIR_PIN, DEFAULT_VALUE_MOTOR1_DIR);
        setMotorDirection(MOTOR2_DIR_PIN, DEFAULT_VALUE_MOTOR2_DIR);
        setMaxStepCount(DEFAULT_VALUE_STEP_COUNT);
        setState(State::Calibrate);
        setEntityId(DEFAULT_ENTITY_ID);
        setMotorCount(DEFAULT_MOTOR_COUNT);
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

void Repository::setMaxStepCount(unsigned short value)
{
    _maxStepCount = value;
    EEPROM.write(STEP_COUNT_ADDR_HIGH, value >> 8);
    EEPROM.write(STEP_COUNT_ADDR_LOW, value & 0x00FF);

    EEPROM.commit();
}

void Repository::setState(State state)
{
    _initialState = state;
    EEPROM.write(STATE_ADDR, state);
    EEPROM.commit();
}

void Repository::setEntityId(String id)
{
    auto entityIdCount = id.length();
    for (uint8_t i = 0; i < entityIdCount; ++i)
    {
        EEPROM.write(ENTITY_ID_ADDR + i, id[i]);
    }

    _entityId = id;
    EEPROM.write(ENTITY_ID_COUNT_ADDR, entityIdCount);
    EEPROM.commit();
}

void Repository::setMotorCount(byte count)
{
    _numberOfMotors = count;
    EEPROM.write(NUMBER_OF_MOTORS_ADDR, count);
    EEPROM.commit();
}