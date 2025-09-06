#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define SSID "TheMadHouse24"
#define PASSWORD "masonlewissuck!"
#define MQTT_SERVER "192.168.1.120"
#define MQ_USER "mqttuser"
#define MQ_PASSWORD "mqttuserpassword"

#define DISCOVERY_TOPIC "homeassistant/device/{ID}/config"
#define COMMAND_TOPIC "home/{ID}/command"
#define STATE_TOPIC "home/{ID}/state"
#define BASE_TOPIC "home"
#define SERIAL_BAUD_RATE 115200
#define VERSION "Version 1.0.0 - Curtain Control"

#define MAGIC_NUMBER 24
#define MAGIC_NUMBER_ADDR 0
#define MOTOR1_DIR_ADDR 1
#define MOTOR2_DIR_ADDR 2
#define STEP_COUNT_ADDR_HIGH 3
#define STEP_COUNT_ADDR_LOW (STEP_COUNT_ADDR_HIGH + 1)
#define STATE_ADDR 5
#define NUMBER_OF_MOTORS_ADDR 6
#define ENTITY_ID_COUNT_ADDR 7
#define ENTITY_ID_ADDR 8

#define DEFAULT_VALUE_MOTOR1_DIR 1
#define DEFAULT_VALUE_MOTOR2_DIR 1
#define DEFAULT_VALUE_STEP_COUNT 3200
#define DEFAULT_MOTOR_COUNT 2
#define DEFAULT_ENTITY_ID "default_entity_id"

#define MOTOR1_STEP_PIN D1
#define MOTOR1_DIR_PIN D7
#define MOTOR1_ENABLE_PIN D3

#define END_STOP_SWITCH1 D5
#define END_STOP_SWITCH2 D6

#define MOTOR2_STEP_PIN D2
#define MOTOR2_DIR_PIN D0
#define MOTOR2_ENABLE_PIN D4

#define STATE_OPEN "OPEN"
#define STATE_OPENING "OPENING"
#define STATE_CLOSED "CLOSED"
#define STATE_CLOSING "CLOSING"
#define STATE_STOPPED "STOPPED"
#define CMD_OPEN "OPEN"
#define CMD_CLOSE "CLOSE"
#define CMD_STOP "STOP"
#endif //!_CONSTANT_H_