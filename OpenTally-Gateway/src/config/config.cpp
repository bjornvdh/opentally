#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "display/display_bootstatus.h"
#include "config/config.h"

StaticJsonDocument<8192> jsonDoc;
JsonArray jsonKeys;
bool configIsValid = false;

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

    DeserializationError error = deserializeJson(jsonDoc, jsonFile);
    jsonFile.close();

    if(error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());        
        configIsValid = false;
        display_bootstepresult(false);
        while(1) yield();   // Halt
    }
    else
    {
        jsonKeys = jsonDoc["key"].to<JsonArray>();
        configIsValid = true;
        display_bootstepresult(true);
    }
}

ConfigAction config_getkeypressedaction(uint8_t numKey)
{
    // TODO: This crashes. Find out why. 
    ConfigAction retVal;
    
    JsonObject theKey = jsonKeys[numKey];

    if(theKey.containsKey("press"))
    {
        JsonObject pressJson = theKey["press"];
        if(pressJson.containsKey("a")) retVal.Action = pressJson["a"];
        if(pressJson.containsKey("p1")) retVal.Action = pressJson["p1"];
        //if(pressJson.containsKey("p2")) retVal.Action = pressJson["p2"];
    }

    return retVal;
}