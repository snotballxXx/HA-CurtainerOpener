/*
  ______                      _           _____
 / _____)           _        (_)         / ___ \
| /     _   _  ____| |_  ____ _ ____    | |   | |____   ____ ____   ____  ____
| |    | | | |/ ___)  _)/ _  | |  _ \   | |   | |  _ \ / _  )  _ \ / _  )/ ___)
| \____| |_| | |   | |_( ( | | | | | |  | |___| | | | ( (/ /| | | ( (/ /| |
 \______)____|_|    \___)_||_|_|_| |_|   \_____/| ||_/ \____)_| |_|\____)_|
                                                |_|
 _  _  _                           _____    __    ______  _       _
| || || |                         (____ \  /  |  |  ___ \(_)     (_)
| || || | ____ ____   ___   ___    _   \ \/_/ |  | | _ | |_ ____  _
| ||_|| |/ _  )    \ / _ \ /___)  | |   | | | |  | || || | |  _ \| |
| |___| ( (/ /| | | | |_| |___ |  | |__/ /  | |  | || || | | | | | |
 \______|\____)_|_|_|\___/(___/   |_____/   |_|  |_||_||_|_|_| |_|_|
 Created by http://www.network-science.de/ascii/ (stop)

  Pin Out                        _______________
                                (               )
                               (       ____      )
                          RST  | o    |    |   o | TX
                          A0   | o    |____|   o | RX
                          D0   | o             o | D1
                          D5   | o             o | D2
                          D6   | o             o | D3
                          D7   | o             o | D4
                          D8   | o             o | GND
                          3v3  | o             o | 5v
                               |      _____      |
                               (     |     |     )
                                (____|     |____)
                                     |_____|

This project connects to HomeAssistant via MQTT, it controls stepper motors that open/close
curtains
*/

#include <Arduino.h>
#include "./control/HomeAssistantMqtt.h"
#include "./control/Wifi.h"
#include "./control/WebServer.h"
#include "./control/Ota.h"
#include "./control/Controller.h"
#include "./control/Repository.h"
#include "constants.h"

Control::Wifi *wifi = new Control::Wifi();
Control::WebServer *webServer = new Control::WebServer();
Control::HomeAssistantMqtt *homeAssistant = new Control::HomeAssistantMqtt();
Control::Ota *ota = new Control::Ota(webServer);
Control::Controller *controller = new Control::Controller(homeAssistant);

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);

    wifi->setup();
    webServer->setup();
    homeAssistant->setup();
    ota->setup();
    controller->setup();
}

void loop()
{
    unsigned long now = millis();

    wifi->loop(now);
    webServer->loop(now);
    homeAssistant->loop(now);
    controller->loop(now);
    ota->loop(now);
}
/*
#include "constants.h"
const int dirPin = MOTOR2_DIR_PIN;
const int stepPin = MOTOR2_STEP_PIN;
const int enablePin = MOTOR2_ENABLE_PIN;
const int switchPin1 = END_STOP_SWITCH1;
const int switchPin2 = END_STOP_SWITCH2;
const int stepsPerRevolution = 200;

void setup()
{
    // Declare pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(switchPin1, INPUT_PULLUP);
    pinMode(switchPin2, INPUT_PULLUP);

    digitalWrite(enablePin, LOW);

    Serial.begin(115200);
}
void loop()
{
    // Set motor direction clockwise
    /*    digitalWrite(dirPin, HIGH);

        // Spin motor slowly
        for (int x = 0; x < stepsPerRevolution; x++)
        {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(1000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(1000);
        }
        delay(1000); // Wait a second

        // Set motor direction counterclockwise
        digitalWrite(dirPin, LOW);

        // Spin motor quickly
        for (int x = 0; x < stepsPerRevolution; x++)
        {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(1000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(1000);
        }
    delay(1000); // Wait a second

    Serial.print("SW1 ");
    Serial.println(digitalRead(switchPin1));
    Serial.print("SW2 ");
    Serial.println(digitalRead(switchPin2));
}
*/