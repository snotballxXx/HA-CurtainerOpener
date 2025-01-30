#include "HomeAssistantMqtt.h"
#include <StreamString.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "interfaces/IWifi.h"
#include "interfaces/ITopicCallback.h"
#include "../constants.h"
#include "../utils/Helpers.h"

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

String discoveryMessage = "{ \
  \"device\": { \
    \"identifiers\": [ \
      \"{ID}\" \
    ], \
    \"name\": \"CurtainDevice\", \
    \"model\": \"CURTAIN-200\", \
    \"manufacturer\": \"DeviceCo\", \
    \"sw_version\": \"1.0\" \
  }, \
  \"origin\": { \
    \"name\": \"{ID}-curtain\" \
  }, \
  \"components\": { \
     \"main_curtain\": { \
       \"platform\": \"cover\", \
       \"device_class\": \"curtain\", \
       \"name\": \"main curtain\", \
       \"command_topic\": \"home/{ID}/command\", \
       \"state_topic\": \"home/{ID}/state\", \
       \"payload_open\": \"OPEN\", \
       \"payload_close\": \"CLOSE\", \
       \"payload_stop\": \"STOP\", \
       \"state_open\": \"OPEN\", \
       \"state_opening\": \"OPENING\", \
       \"state_closed\": \"CLOSED\", \
       \"state_closing\": \"CLOSING\", \
       \"state_stopped\": \"STOPPED\", \
       \"icon_template\": \"{% if is_state(this, 'open') %} mdi:curtains-closed {% else %} mdi:curtains {% endif %}\", \
       \"unique_id\": \"{ID}-curtain\" \
     } \
  } \
}";

void callback(char *topic, byte *payload, unsigned int length, void *p)
{
  String content;
  content.reserve(length);

  for (unsigned int i = 0; i < length; i++)
    content += (char)payload[i];

  ((HomeAssistantMqtt *)p)->invokeCallbacks(String(topic), content);
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
  _client->setCallback(callback, (void *)this);

  _clientId = Utils::Helpers::composeClientID();
  _clientId.replace(':', '-');
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
    Serial.println("Request to send on " + tmp);
    Serial.println("Publish on " + tmp + " with message " + msg);
    _client->publish(tmp.c_str(), msg.c_str(), true);
  }
}

void HomeAssistantMqtt::subscribe(const String &topic, Interfaces::ITopicCallback *callback)
{
  String tmp(topic);
  tmp.replace("{ID}", _clientId);
  Serial.println("Subscribed to " + tmp);
  _callbacks.insert(std::pair<String, Interfaces::ITopicCallback *>(tmp, callback));
}

void HomeAssistantMqtt::invokeCallbacks(const String &topic, const String &payload)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(": ");
  Serial.println(payload);

  auto range = _callbacks.equal_range(topic);

  for (auto it = range.first; it != range.second; ++it)
    it->second->messageReceived(topic, payload);
}

void HomeAssistantMqtt::reconnect()
{
  // Loop until we're reconnected
  while (!_client->connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (_client->connect(_clientId.c_str(), MQ_USER, MQ_PASSWORD))
    {
      Serial.println("connected");
      String topic = DISCOVERY_TOPIC;
      topic.replace("{ID}", _clientId);

      String msg = discoveryMessage;
      msg.replace("{ID}", _clientId);

      Serial.println("Message published to topic (2k) '" + topic + "' :" + msg);
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
      Serial.print("failed, rc=");
      Serial.print(_client->state());
      Serial.print(" wifi=");
      Serial.print(WiFi.status());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}