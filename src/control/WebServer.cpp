#include "./WebServer.h"
#include "../control/Repository.h"
#include "constants.h"
#include "../utils/Helpers.h"

using namespace Control;

#define MOTOR1_BOOL "{MOTOR1_BOOL}"
#define MOTOR2_BOOL "{MOTOR2_BOOL}"
#define MOTOR1 "{MOTOR1}"
#define MOTOR2 "{MOTOR2}"
#define COUNT "{COUNT}"
#define ENTITY_ID "{ENTITY_ID}"
#define VERSION_TAG "{VERSION}"

extern Control::WebServer *webServer;
extern FileSystem *fileSystem;

void handlePost()
{
    Serial.print("Received POST");
    if (webServer->getServer()->hasArg("plain"))
    {
        // motor1=1&motor2=0&stepCount=5000&entity_id=test
        String body = webServer->getServer()->arg("plain");
        Serial.print("Received POST data: ");
        Serial.println(body);

        byte motor1 = 0;
        if (body.indexOf("motor1=1") != -1)
            motor1 = 1;

        byte motor2 = 0;
        if (body.indexOf("motor2=1") != -1)
            motor2 = 1;

        auto stepCountPart = Utils::Helpers::getValue(body, '&', 2);
        unsigned short stepCount = stepCountPart.substring(stepCountPart.indexOf('=') + 1).toInt();
        auto entityIdPart = Utils::Helpers::getValue(body, '&', 3);
        auto entityId = entityIdPart.substring(entityIdPart.indexOf('=') + 1);

        auto repo = Repository::getInstance();
        repo->setMaxStepCount(stepCount);
        repo->setMotorDirection(MOTOR1_DIR_PIN, motor1);
        repo->setMotorDirection(MOTOR2_DIR_PIN, motor2);
        repo->setEntityId(entityId);
        webServer->getServer()->send(200, "text/html", fileSystem->getConfirmPage());
    }
}

WebServer::WebServer(FileSystem *fileSystem) : _webServer(nullptr), _fileSystem(fileSystem)
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
        page.replace(VERSION_TAG, output);      
        ws->send(200, "text/html", page); });

    _webServer->on("/configUpdate", HTTP_POST, handlePost);
}

void WebServer::loop(unsigned long time)
{
    _webServer->handleClient();
}