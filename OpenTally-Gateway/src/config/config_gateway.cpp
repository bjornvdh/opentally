#include <Arduino.h>
#include <ArduinoJSON.h>
#include <SPIFFS.h>
#include "display/display_bootstatus.h"
#include "config/config_gateway.h"
#include "config/config_enums.h"
#include "config/config_structs.h"

bool configIsValid = false;

KeyConfig _keys[18];
LEDConfig _leds[16];

void config_gateway_setup()
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

    if(root.containsKey("led"))
    {
        Serial.println("[config] Found led element.");
        for(int i = 0; i < 16; i++)
        {
            JsonObject jsonLed = root["led"][i];
            _leds[i].BaseColor = jsonLed["bc"].as<bool>();

            _leds[i].SolidColor.Mode = (LedMode)jsonLed["c"]["m"].as<int>();
            _leds[i].SolidColor.Param1 = jsonLed["c"]["p1"].as<int>();

            _leds[i].Blink.Mode = (LedMode)jsonLed["bl"]["m"].as<int>();
            _leds[i].Blink.Param1 = jsonLed["bl"]["p1"].as<int>();
        }        
    }
    else
    {
        Serial.println("Led element not found in config JSON.");
        display_bootstepresult(false);
        while(1) yield();   // Halt        
    }

    if(root.containsKey("key"))
    {
        Serial.println("[config] Found key element.");
        for(int i = 0; i < 18; i++)
        {
            JsonObject jsonKey = root["key"][i];
            _keys[i].OnPress.Action = (KeyAction)jsonKey["p"]["a"].as<int>();
            _keys[i].OnPress.Param1 = jsonKey["p"]["p1"].as<int>();
            _keys[i].OnPress.Param2 = jsonKey["p"]["p2"].as<String>();

            _keys[i].OnLongpress.Action = (KeyAction)jsonKey["lp"]["a"].as<int>();
            _keys[i].OnLongpress.Param1 = jsonKey["lp"]["p1"].as<int>();
            _keys[i].OnLongpress.Param2 = jsonKey["lp"]["p2"].as<String>();
        }
    }
    else
    {
        Serial.println("Key element not found in config JSON.");
        display_bootstepresult(false);
        while(1) yield();   // Halt        
    }
    
    display_bootstepresult(true);
}

KeyConfig config_getkeyconfig(uint8_t numKey)
{   
    return _keys[numKey];
}

LEDConfig config_getledconfig(uint8_t numLed)
{   
    return _leds[numLed];
}