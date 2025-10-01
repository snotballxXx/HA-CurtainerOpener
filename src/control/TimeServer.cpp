#include "TimeServer.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <constants.h>
#include <interfaces/ILogger.h>

using namespace Control;

extern Interfaces::ILogger* logger;

TimeServer::TimeServer(Interfaces::IMessenger* messenger)
    : _messenger(messenger),
      _time(0),
      _lastUpdateRequest(0),
      _currentTime(0),
      _timeOffset(0),
      _lastRequestTime(0),
      _baseTime(0)
{
}

void TimeServer::setup()
{
    _messenger->subscribe(TIME_SERVER_TOPIC, this);
    logger->sendLog("Subscribing to time service");
}

void TimeServer::loop(unsigned long time)
{
    if (_time == 0)
    {
        if (_lastUpdateRequest < time)
        {
            _lastUpdateRequest += 5000;
            _messenger->sendMessage(TIME_SERVER_TOPIC_REQ, "update please...");
            logger->sendLog("Update request");
        }
    }
    else
    {
        _time = _baseTime + (time - _timeOffset);
    }
}

void TimeServer::messageReceived(const String& topic, const String& payload)
{
    logger->sendLog("Time update " + payload);
    _time = _baseTime = ((uint64_t) payload.toInt()) * 1000;
    _timeOffset       = (uint64_t) millis();
    logger->sendLog(String("Time update: " + String(_time)));
}

String TimeServer::getTimeAsString(unsigned long requestTime)
{
    if (_time == 0) return "NO TIME";

    if (requestTime == _lastRequestTime) return _lastRequestAsString;

    time_t epoch_sec   = _time / 1000;
    int    millis_part = _time % 1000;

    struct tm* tm_info = gmtime(&epoch_sec);
    char       buffer[32];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", tm_info);

    char millis_str[4];
    sprintf(millis_str, "%03d", millis_part);

    if (requestTime != 0)
    {
        _lastRequestTime     = requestTime;
        _lastRequestAsString = String(buffer) + "." + String(millis_str);
    }

    return _lastRequestAsString;
}

unsigned long TimeServer::getTime()
{
    return _time;
}