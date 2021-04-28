#include <Arduino.h>
#include <Arduino_JSON.h>
#include <SPIFFS.h>
#include "display/display_bootstatus.h"
#include "config/config.h"

bool configIsValid = false;

KeyConfig _keys[18];

void config_setup()
{
    display_bootstep("Loading config...");
    File jsonFile = SPIFFS.open("/config.json");
    if(!jsonFile)
    {
        configIsValid = false;
        display_bootstepresult(false);
        while(1) yield();   // Halt
    }

    JSONVar root = JSON.parse(jsonFile.readString());
    jsonFile.close();

    if(root.hasOwnProperty("key"))
    {
        for(int i = 0; i < 18; i++)
        {
            JSONVar jsonKey = root["key"][i];
            _keys[i].OnPress.Action = jsonKey["p"]["a"];
            _keys[i].OnPress.ActionParam1 = jsonKey["p"]["p1"];
            _keys[i].OnPress.ActionParam2 = jsonKey["p"]["p2"];
        }
        display_bootstepresult(true);
    }
    else
    {
        Serial.println("Key element not found in config JSON.");
        display_bootstepresult(false);
        while(1) yield();   // Halt        
    }
}

KeyConfig config_getkeyconfig(uint8_t numKey)
{   
    return _keys[numKey];
}