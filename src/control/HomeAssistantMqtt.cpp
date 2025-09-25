#include "HomeAssistantMqtt.h"
#include <StreamString.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "interfaces/IWifi.h"
#include "interfaces/ITopicCallback.h"
#include "../constants.h"
#include "../utils/Helpers.h"
#include "./Repository.h"
#include <FileSystem.h>
#include <interfaces/ILogger.h>

/*
The state of a cover
A cover can have the following states:

Opening: The cover is in the process of opening to reach a set position.
Open: The cover has reached the open position.
Closing: The cover is in the process of closing to reach a set position.
Closed: The cover has reached the closed position.
Unavailable: The entity is currently unavailable.
Unknown: The state is not yet known.
*/

using namespace Control;
extern Interfaces::ILogger *logger;
extern FileSystem *fileSystem;
extern HomeAssistantMqtt *homeAssistant;

void callback(char *topic, byte *payload, unsigned int length)
{
  String content;
  content.reserve(length);

  for (unsigned int i = 0; i < length; i++)
    content += (char)payload[i];

  homeAssistant->invokeCallbacks(String(topic), content);
}

HomeAssistantMqtt::HomeAssistantMqtt()
{
}

void HomeAssistantMqtt::setup()
{
  _espClient = new WiFiClient;
  _client = new PubSubClient(*_espClient);

  _client->setBufferSize(3000);
  _client->setServer(MQTT_SERVER, 1883);
  _client->setCallback(callback);

  _clientId = Utils::Helpers::composeClientID();
  _clientId.replace(':', '-');

  _entityId = Repository::getInstance()->getEntityId();
}

void HomeAssistantMqtt::loop(unsigned long time)
{
  // confirm still connected to mqtt server
  if (!_client->connected())
    reconnect();

  _client->loop();
}

void HomeAssistantMqtt::sendMessage(const String &topic, const String &msg)
{
  if (_client->connected())
  {
    String tmp = topic;
    tmp.replace("{ID}", _clientId);
    logger->sendLog("Request to send on " + tmp);
    logger->sendLog("Publish on " + tmp + " with message " + msg);
    _client->publish(tmp.c_str(), msg.c_str(), true);
  }
}

void HomeAssistantMqtt::subscribe(const String &topic, Interfaces::ITopicCallback *callback)
{
  String tmp(topic);
  tmp.replace("{ID}", _clientId);
  logger->sendLog("Subscribed to " + tmp);
  _callbacks.insert(std::pair<String, Interfaces::ITopicCallback *>(tmp, callback));
}

void HomeAssistantMqtt::invokeCallbacks(const String &topic, const String &payload)
{
  logger->sendLog("Message arrived [" + topic + "]: " + payload);

  auto range = _callbacks.equal_range(topic);

  for (auto it = range.first; it != range.second; ++it)
    it->second->messageReceived(topic, payload);
}

void HomeAssistantMqtt::reconnect()
{
  // Loop until we're reconnected
  while (!_client->connected())
  {
    logger->sendLog("Attempting MQTT connection...");

    // Attempt to connect
    if (_client->connect(_clientId.c_str(), MQ_USER, MQ_PASSWORD))
    {
      logger->sendLog("connected");
      String topic = DISCOVERY_TOPIC;
      topic.replace("{ID}", _clientId);

      String msg = fileSystem->getDiscovery();
      msg.replace("{ID}", _clientId);
      msg.replace("{ENTITY_ID}", _entityId);

      logger->sendLog("Message published to topic (2k) '" + topic + "' :" + msg);
      // Once connected, publish a discovery message
      _client->publish(topic.c_str(), msg.c_str(), true);

      for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it)
      {
        _client->unsubscribe(it->first.c_str());
        _client->subscribe(it->first.c_str());
      }
    }
    else
    {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}