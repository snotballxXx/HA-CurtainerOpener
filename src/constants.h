#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define SSID                "TheMadHouse24"
#define PASSWORD            "masonlewissuck!"
#define MQTT_SERVER         "192.168.1.120"
#define MQ_USER             "mqttuser"
#define MQ_PASSWORD         "mqttuserpassword"

#define DISCOVERY_TOPIC     "homeassistant/device/{ID}/config"
#define COMMAND_TOPIC       "home/{ID}/command"
#define STATE_TOPIC         "home/{ID}/state"
#define BASE_TOPIC          "home"
#define SERIAL_BAUD_RATE     115200
#define VERSION              "Version 1.0.0 - Curtain Control"

#define MAGIC_NUMBER                     23
#define MAGIC_NUMBER_ADDR                 0
#define MOTOR1_DIR_ADDR                   1
#define MOTOR2_DIR_ADDR                   2
#define STEP_COUNT_ADDR_HIGH              3
#define STEP_COUNT_ADDR_LOW               (STEP_COUNT_ADDR_HIGH + 1)

#define DEFAULT_VALUE_MOTOR1_DIR              1
#define DEFAULT_VALUE_MOTOR2_DIR              1
#define DEFAULT_VALUE_STEP_COUNT           3200

#endif  //!_CONSTANT_H_