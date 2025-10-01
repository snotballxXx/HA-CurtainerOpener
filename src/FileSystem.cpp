#include "./FileSystem.h"
#include <LittleFS.h>

FileSystem::FileSystem() : _settingsPage("Not Found"), _discovery("Not Found"), _messagesPage("Not Found") {}

void FileSystem::setup()
{
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS mount failed");
        return;
    }

    File file = LittleFS.open("/settings.html", "r");
    if (!file)
    {
        Serial.println("Failed to open file /settings.html");
    }
    else
    {
        _settingsPage = file.readString();  // Reads entire file into a String
        file.close();
    }

    file = LittleFS.open("/message.html", "r");
    if (!file)
    {
        Serial.println("Failed to open file /message.html");
    }
    else
    {
        _messagesPage = file.readString();  // Reads entire file into a String
        file.close();
    }

    file = LittleFS.open("/discovery.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file /discovery.json");
    }
    else
    {
        _discovery = file.readString();  // Reads entire file into a String
        file.close();
    }
}

void FileSystem::loop(unsigned long time) {}