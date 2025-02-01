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
#include "./control/CurtainControl.h"
#include "./control/Repository.h"
#include "constants.h"

Control::Wifi* wifi = new Control::Wifi();
Control::WebServer* webServer = new Control::WebServer();
Control::HomeAssistantMqtt* homeAssistant = new Control::HomeAssistantMqtt();
Control::Ota* ota = new Control::Ota(webServer);
Control::CurtainControl* curtain = new Control::CurtainControl(homeAssistant);

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    
    wifi->setup();
    webServer->setup();
    homeAssistant->setup();
    ota->setup();
    curtain->setup();
}

void loop()
{
    unsigned long now = millis();

    wifi->loop(now);
    webServer->loop(now);
    homeAssistant->loop(now);
    curtain->loop(now);
    ota->loop(now);
}