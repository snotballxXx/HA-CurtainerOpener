#include "Wifi.h"
#include <ESP8266WiFi.h>
#include "constants.h"

using namespace Control;

void Wifi::setup()
{
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void Wifi::loop(unsigned long time)
{
}