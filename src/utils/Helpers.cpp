
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

String Helpers::getValue(String data, char separator, int index)
{
    int found      = 0;
    int strIndex[] = {0, -1};
    int maxIndex   = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String Helpers::stateToString(Control::State state)
{
    switch (state)
    {
        case Control::Open:
            return "Open";
        case Control::Opening:
            return "Opening";
        case Control::Closing:
            return "Closing";
        case Control::Closed:
            return "Closed";
        case Control::Stopped:
            return "Stopped";
        case Control::PendingChange:
            return "PendingChange";
        case Control::Calibrate:
            return "Calibrate";
        default:
            return "";
    }
}