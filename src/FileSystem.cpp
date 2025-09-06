#include "./FileSystem.h"
#include <LittleFS.h>

FileSystem::FileSystem() : _settingsPage("Not Found"),
                           _discovery("Not Found")
{
}

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
        _settingsPage = file.readString(); // Reads entire file into a String
        Serial.println("File content:");
        Serial.println(_settingsPage);
        file.close();
    }

    file = LittleFS.open("/discovery.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file /discovery.json");
    }
    else
    {
        _discovery = file.readString(); // Reads entire file into a String
        Serial.println("File content:");
        Serial.println(_discovery);
        file.close();
    }
}

void FileSystem::loop(unsigned long time)
{
}