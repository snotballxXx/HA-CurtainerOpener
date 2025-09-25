#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "./interfaces/IArduinoBase.h"
#include <StreamString.h>

class FileSystem : public Interfaces::IAduninoBase
{
    String _settingsPage;
    String _discovery;
    String _messagesPage;

    public:
    FileSystem();
    
    virtual void loop(unsigned long time);
    virtual void setup();

    String getSettingsPage() { return _settingsPage; }
    String getMessagesPage() { return _messagesPage; }
    String getDiscovery() { return _discovery; }
};

#endif //_FILE_SYSTEM_H_