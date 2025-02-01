#include <Arduino.h>
#include "./Ota.h"
#include <ElegantOTA.h>
#include "../interfaces/IWebServer.h"

using namespace Control;

Ota::Ota(Interfaces::IWebServer* webserver) :
_webserver(webserver)
{
}

void Ota::setup()
{
    ElegantOTA.begin(_webserver->getServer());
}

void Ota::loop(unsigned long time)
{
    ElegantOTA.loop();
}