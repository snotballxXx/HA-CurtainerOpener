#include "./WebServer.h"
#include "../control/Repository.h"
#include "constants.h"
#include "../utils/Helpers.h"
#include "../interfaces/ITopicCallback.h"

using namespace Control;

#define MOTOR1_BOOL "{MOTOR1_BOOL}"
#define MOTOR2_BOOL "{MOTOR2_BOOL}"
#define MOTOR1 "{MOTOR1}"
#define MOTOR2 "{MOTOR2}"
#define COUNT "{COUNT}"
#define ENTITY_ID "{ENTITY_ID}"
#define MOTOR_COUNT "{MOTOR_COUNT}"
#define VERSION_TAG "{VERSION}"

extern Control::WebServer *webServer;
extern FileSystem *fileSystem;

void handleActionPost()
{
    auto body = webServer->getServer()->arg("plain"); // Gets the raw body text
    Serial.println("Received body:");
    Serial.println(body);

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

void handlePost()
{
    if (webServer->getServer()->hasArg("motor1") &&
        webServer->getServer()->hasArg("motor2") &&
        webServer->getServer()->hasArg("stepCount") &&
        webServer->getServer()->hasArg("entity_id") &&
        webServer->getServer()->hasArg("motorCount"))
    {
        Serial.println("Received POST:SUCCESS");
        auto motor1 = webServer->getServer()->arg("motor1") == "1" ? 1 : 0;
        auto motor2 = webServer->getServer()->arg("motor2") == "1" ? 1 : 0;
        auto stepCount = webServer->getServer()->arg("stepCount").toInt();
        auto entityId = webServer->getServer()->arg("entity_id");
        auto motorCount = webServer->getServer()->arg("motorCount").toInt();

        auto repo = Repository::getInstance();
        repo->setMaxStepCount(stepCount);
        repo->setMotorDirection(MOTOR1_DIR_PIN, motor1);
        repo->setMotorDirection(MOTOR2_DIR_PIN, motor2);
        repo->setEntityId(entityId);
        repo->setMotorCount(motorCount);
        webServer->getServer()->send(200, "text/html", "SUCCESS");
    }
    else
    {
        Serial.println("Received POST: FAILED");
        webServer->getServer()->send(400, "text/html", "FAILED");
    }
}

WebServer::WebServer(FileSystem *fileSystem, Interfaces::ITopicCallback *actionCallback) : _webServer(nullptr),
                                                                                           _fileSystem(fileSystem),
                                                                                           _actionCallback(actionCallback)
{
    _webServer = new ESP8266WebServer(80);
}

void WebServer::setup()
{
    Serial.println("HTTP server started");

    _webServer->begin();

    auto ws = _webServer;
    auto fs = _fileSystem;
    _webServer->on("/", [ws, fs]()
                   {
        auto r = Repository::getInstance();
        String output = String(VERSION) + String(" ") + String(Utils::Helpers::composeClientID());
        String page = fs->getSettingsPage();
        page.replace(MOTOR1_BOOL, r->getMotorDirection(MOTOR1_DIR_PIN) == 0 ? "" : "checked");
        page.replace(MOTOR2_BOOL, r->getMotorDirection(MOTOR2_DIR_PIN) == 0 ? "" : "checked");

        page.replace(MOTOR1, String(r->getMotorDirection(MOTOR1_DIR_PIN)));
        page.replace(MOTOR2, String(r->getMotorDirection(MOTOR2_DIR_PIN)));

        page.replace(ENTITY_ID, String(r->getEntityId()));

        page.replace(COUNT, String(r->getMaxStepCount()));
        page.replace(MOTOR_COUNT, String(r->getMotorCount()));
        page.replace(VERSION_TAG, output);      
        ws->send(200, "text/html", page); });

    _webServer->on("/configUpdate", HTTP_POST, handlePost);

    _webServer->on("/actionUpdate", HTTP_POST, handleActionPost);
}

void WebServer::loop(unsigned long time)
{
    _webServer->handleClient();
}