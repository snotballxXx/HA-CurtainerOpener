#include "./WebServer.h"
#include "../htmlFiles.h"
#include "../control/Repository.h"
#include "constants.h"
#include "../utils/Helpers.h"

using namespace Control;

#define MOTOR_BOOL      "{MOTOR_BOOL}"
#define MOTOR          "{MOTOR}"
#define COUNT           "{COUNT}"
#define VERSION_TAG     "{VERSION}"

extern Control::WebServer* webServer;

void handlePost()
{
    Serial.print("Received POST");
    if (webServer->getServer()->hasArg("plain")) 
    { 
        String body = webServer->getServer()->arg("plain"); 
        Serial.print("Received POST data: "); 
        Serial.println(body);

        byte motorDir = 0;
        if (body.indexOf("motor=1") != -1)
            motorDir = 1;

        unsigned short stepCount = body.substring(body.lastIndexOf('=') + 1).toInt();
        auto repo = Repository::getInstance();
        
        repo->setCloseStepCount(stepCount);
        repo->setMotorDirection(motorDir);       
        webServer->getServer()->send(200, "text/html", response);
    }
}

WebServer::WebServer() :
_webServer(nullptr)
{
    _webServer = new ESP8266WebServer(80);
}

void WebServer::setup()
{
    Serial.println("HTTP server started");
    
    _webServer->begin();

    ESP8266WebServer* ws = _webServer;
    _webServer->on("/", [ws]() {

        auto r = Repository::getInstance();
        String output = String(VERSION) + String(" ") + String(Utils::Helpers::composeClientID());
        String page = configPage;
        page.replace(MOTOR_BOOL, r->getMotorDirection() == 0 ? "" : "checked");

        page.replace(MOTOR, String(r->getMotorDirection()));

        page.replace(COUNT, String(r->getCloseStepCount()));
        page.replace(VERSION_TAG, output);      
        ws->send(200, "text/html", page); 
    });

    _webServer->on("/configUpdate", HTTP_POST, handlePost);
}

void WebServer::loop(unsigned long time)
{
    _webServer->handleClient();
}