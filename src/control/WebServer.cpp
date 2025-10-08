#include "./WebServer.h"
#include "../control/Repository.h"
#include "../interfaces/ITopicCallback.h"
#include "../utils/Helpers.h"
#include "./Repository.h"
#include "TimeServer.h"
#include "constants.h"

using namespace Control;

#define MOTOR1_BOOL "{MOTOR1_BOOL}"
#define MOTOR2_BOOL "{MOTOR2_BOOL}"
#define MOTOR1 "{MOTOR1}"
#define MOTOR2 "{MOTOR2}"
#define COUNT "{COUNT}"
#define ENTITY_ID "{ENTITY_ID}"
#define MOTOR_COUNT "{MOTOR_COUNT}"
#define VERSION_TAG "{VERSION}"
#define LOG_TO_MQ_BOOL "{LOG_TO_MQ_BOOL}"
#define LOG_TO_MQ "{LOG_TO_MQ}"

extern WebServer*               webServer;
extern FileSystem*              fileSystem;
extern Interfaces::ILogger*     logger;
extern Interfaces::ITimeServer* timeServer;

void handleActionPost()
{
    auto body = webServer->getServer()->arg("plain");  // Gets the raw body text
    logger->sendLog("Received body:");
    logger->sendLog(body);

    if (body == CMD_OPEN)
    {
        webServer->getCallback()->messageReceived("", body);
        return webServer->getServer()->send(200, "text/html", "SUCCESS");
    }
    else if (body == CMD_CLOSE)
    {
        webServer->getCallback()->messageReceived("", body);
        return webServer->getServer()->send(200, "text/html", "SUCCESS");
    }
    else if (body == CMD_STOP)
    {
        webServer->getCallback()->messageReceived("", body);
        return webServer->getServer()->send(200, "text/html", "SUCCESS");
    }

    webServer->getServer()->send(400, "text/html", "FAILED");
}

void handleSettingsPost()
{
    if (webServer->getServer()->hasArg("motor1") && webServer->getServer()->hasArg("motor2") &&
        webServer->getServer()->hasArg("stepCount") && webServer->getServer()->hasArg("entity_id") &&
        webServer->getServer()->hasArg("motorCount") && webServer->getServer()->hasArg("logToMq"))
    {
        logger->sendLog("Received POST:");
        auto msg = String();

        auto motor1 = webServer->getServer()->arg("motor1") == "1" ? 1 : 0;
        msg.concat("motor1=" + webServer->getServer()->arg("motor1"));
        msg.concat("\n");
        auto motor2 = webServer->getServer()->arg("motor2") == "1" ? 1 : 0;
        msg.concat("motor2=" + webServer->getServer()->arg("motor2"));
        msg.concat("\n");
        auto stepCount = webServer->getServer()->arg("stepCount").toInt();
        msg.concat("stepCount=" + webServer->getServer()->arg("stepCount"));
        msg.concat("\n");
        auto entityId = webServer->getServer()->arg("entity_id");
        msg.concat("entity_id=" + entityId);
        msg.concat("\n");
        auto motorCount = webServer->getServer()->arg("motorCount").toInt();
        msg.concat("motorCount=" + webServer->getServer()->arg("motorCount"));
        msg.concat("\n");
        auto logMq = webServer->getServer()->arg("logToMq") == "1" ? 1 : 0;
        msg.concat("logToMq=" + webServer->getServer()->arg("logToMq"));

        logger->sendLog(msg);
        auto repo = Repository::getInstance();
        repo->setMaxStepCount(stepCount);
        repo->setMotorDirection(MOTOR1_DIR_PIN, motor1);
        repo->setMotorDirection(MOTOR2_DIR_PIN, motor2);
        repo->setEntityId(entityId);
        repo->setMotorCount(motorCount);
        repo->setLogToMq(logMq);
        webServer->getServer()->send(200, "text/html", "SUCCESS");
    }
    else
    {
        logger->sendLog("Received POST: FAILED");
        webServer->getServer()->send(400, "text/html", "FAILED");
    }
}

int parseSliderValue(String json)
{
    //{"slider":"994"}
    int start = json.indexOf(":");
    int end   = json.indexOf("}");
    if (start != -1 && end != -1)
    {
        auto value = json.substring(start + 2, end - 1);
        logger->sendLog("Received: " + value);
        return value.toInt();
    }
    return -1;
}

void handleWebSocketMessage(uint8_t client_num, WStype_t type, uint8_t* payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        auto msg = String((char*) payload);

        if (msg.startsWith("{") && msg.endsWith("}"))
        {
            int sliderPos = parseSliderValue(msg);
            webServer->setSlider(sliderPos);
        }
    }
}

WebServer::WebServer(FileSystem* fileSystem, Interfaces::ITopicCallback* actionCallback,
                     Interfaces::IMessenger* messenger)
    : _webServer(nullptr), _fileSystem(fileSystem), _actionCallback(actionCallback), _slider(1), _messenger(messenger)
{
    _webServer = new ESP8266WebServer(80);
    _webSocket = new WebSocketsServer(81);
}

void WebServer::setup()
{
    Serial.println("HTTP server started");

    _webServer->begin();

    auto ws = _webServer;
    auto fs = _fileSystem;
    _webServer->on("/",
                   [ws, fs]()
                   {
                       auto   r      = Repository::getInstance();
                       String output = String(VERSION) + String(" ") + String(Utils::Helpers::composeClientID());
                       String page   = fs->getSettingsPage();
                       page.replace(MOTOR1_BOOL, r->getMotorDirection(MOTOR1_DIR_PIN) == 0 ? "" : "checked");
                       page.replace(MOTOR2_BOOL, r->getMotorDirection(MOTOR2_DIR_PIN) == 0 ? "" : "checked");
                       page.replace(LOG_TO_MQ_BOOL, r->getLogToMq() == 0 ? "" : "checked");

                       page.replace(MOTOR1, String(r->getMotorDirection(MOTOR1_DIR_PIN)));
                       page.replace(MOTOR2, String(r->getMotorDirection(MOTOR2_DIR_PIN)));
                       page.replace(LOG_TO_MQ, String(r->getLogToMq()));

                       page.replace(ENTITY_ID, String(r->getEntityId()));

                       page.replace(COUNT, String(r->getMaxStepCount()));
                       page.replace(MOTOR_COUNT, String(r->getMotorCount()));
                       page.replace(VERSION_TAG, output);
                       ws->send(200, "text/html", page);
                   });

    _webServer->on("/messages",
                   [ws, fs]()
                   {
                       auto   r      = Repository::getInstance();
                       String output = String(VERSION) + String(" ") + String(Utils::Helpers::composeClientID());
                       String page   = fs->getMessagesPage();

                       page.replace(ENTITY_ID, String(r->getEntityId()));
                       page.replace(VERSION_TAG, output);
                       ws->send(200, "text/html", page);
                   });

    _webServer->on("/configUpdate", HTTP_POST, handleSettingsPost);

    _webServer->on("/actionUpdate", HTTP_POST, handleActionPost);

    _webSocket->onEvent(handleWebSocketMessage);

    _webSocket->begin();
}

void WebServer::loop(unsigned long time)
{
    _webServer->handleClient();
    _webSocket->loop();
}

int WebServer::getConnectedWebSocketCount()
{
    return _webSocket->connectedClients();
}

void WebServer::broadcastTXT(const String& msg)
{
    _webSocket->broadcastTXT(msg.c_str());
}
