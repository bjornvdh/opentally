#include <Arduino.h>
#include <ArduinoJSON.h>
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

    DynamicJsonDocument root(8192);
    deserializeJson(root, jsonFile);
    jsonFile.close();

    if(root.containsKey("key"))
    {
        Serial.println("[config] Found key element.");
        for(int i = 0; i < 18; i++)
        {
            Serial.print("[config] Settings for this key: ");
            JsonObject jsonKey = root["key"][i];
            _keys[i].OnPress.Action = jsonKey["p"]["a"].as<int>();
            _keys[i].OnPress.ActionParam1 = jsonKey["p"]["p1"].as<int>();
            _keys[i].OnPress.ActionParam2 = jsonKey["p"]["p2"].as<String>();
            Serial.print(_keys[i].OnPress.ActionParam2);
            Serial.println(".");            
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