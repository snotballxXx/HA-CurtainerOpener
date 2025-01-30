#ifndef _MQTT_H_
#define _MQTT_H_
#include "../interfaces/IMessenger.h"
#include "../interfaces/IArduinoBase.h"
#include <map>

class WiFiClient;
class PubSubClient;

namespace Interfaces
{
    class ITopicCallback;
}

namespace Control
{
    class HomeAssistantMqtt : public Interfaces::IMessenger, public Interfaces::IAduninoBase
    {
        WiFiClient *_espClient;
        PubSubClient *_client;
        String _clientId;
        std::multimap<String, Interfaces::ITopicCallback *> _callbacks;

        void reconnect();

    public:
        HomeAssistantMqtt();

        virtual void loop(unsigned long time);
        virtual void setup();
        virtual void sendMessage(const String &topic, const String &msg);
        virtual void subscribe(const String &topic, Interfaces::ITopicCallback *callback);
        void invokeCallbacks(const String &topic, const String &payload);
    };
}

#endif //!_MQTT_H_