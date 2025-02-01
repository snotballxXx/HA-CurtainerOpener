#include "./WebServer.h"
#include "../htmlFiles.h"
#include "../control/Repository.h"
#include "constants.h"
#include "../utils/Helpers.h"

using namespace Control;

#define MOTOR1_BOOL     "{MOTOR1_BOOL}"
#define MOTOR2_BOOL     "{MOTOR2_BOOL}"
#define MOTOR1          "{MOTOR1}"
#define MOTOR2          "{MOTOR2}"
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

        byte motor1 = 0;
        if (body.indexOf("motor1=1") != -1)
            motor1 = 1;

        byte motor2 = 0;
        if (body.indexOf("motor2=1") != -1)
            motor2 = 1;

        unsigned short stepCount = body.substring(body.lastIndexOf('=') + 1).toInt();
        auto repo = Repository::getInstance();
        
        repo->setCloseStepCount(stepCount);
        repo->setMotor1Direction(motor1);
        repo->setMotor2Direction(motor2);        
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
        page.replace(MOTOR1_BOOL, r->getMotor1Direction() == 0 ? "" : "checked");
        page.replace(MOTOR2_BOOL, r->getMotor2Direction() == 0 ? "" : "checked");

        page.replace(MOTOR1, String(r->getMotor1Direction()));
        page.replace(MOTOR2, String(r->getMotor2Direction()));

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