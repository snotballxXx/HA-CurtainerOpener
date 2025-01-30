
#include <ESP8266WiFi.h>
#include "helpers.h"
#include "macConverter.h"

using namespace Utils;

String Helpers::composeClientID() 
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String clientId;
    clientId += "esp-";
    clientId += macToStr(mac);
    return clientId;
}
