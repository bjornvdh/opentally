#include <Arduino.h>
#include "net/net.h"
#include "display/display.h"
#include "display/display_bootstatus.h"
#include "sleep/sleep.h"

#include <Preferences.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Wifi.h>
#include <WifiManager.h>

#define RSSI_POLL_INTERVAL 2500

WiFiManager wm;
unsigned long _last_rssi_poll = 0;

bool configApIsActive = false;
bool wifiIsConntected = false;
uint8_t wifiRSSI = 0;


uint8_t net_getWifiRSSI()
{
    return wifiRSSI;
}

bool net_configApIsActive()
{
    return configApIsActive;
}

bool net_wifiIsConnected()
{
    return wifiIsConntected;
}


void net_setup()
{
    display_bootstep("Starting WiFi...");
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    
    wm.setShowInfoErase(false);
    wm.setHostname("OpenTallyGW");
    wm.setConfigPortalBlocking(false);
    wm.setWiFiAutoReconnect(true);
    wm.setTitle("OpenTally Gateway");

    if(wm.autoConnect("OpenTally Gateway")){
        Serial.println("connected...yay :)");
    }
    else {
        Serial.println("Configportal running");
    } 

    display_bootstepresult(true);
}

void net_task(void* parameters)
{
    while(true)
    {
        if(sleepInitiated) {
            taskYIELD();
            continue;  // Do not respond to WiFi events when going to sleep.
        }

        wm.process();
        if(!wm.getWebPortalActive())
            wm.startWebPortal();

        if(wm.getConfigPortalActive())
        {
            if(!configApIsActive)
            {
                Serial.println("Config AP is now active.");
                display_request_refresh(false);
            }
            configApIsActive = true;
            wifiIsConntected = false;
        }
        else
        {
            configApIsActive = false;
        }

        if(WiFi.getMode() == WIFI_MODE_STA && WiFi.isConnected())
        {
            if(!wifiIsConntected)
            {
                Serial.println("Wifi is now connected.");
                display_request_refresh(false);
            }
            wifiIsConntected = true;
            configApIsActive = false;

            if((millis() - _last_rssi_poll) > RSSI_POLL_INTERVAL)
            {
                wifiRSSI = WiFi.RSSI();
                display_request_refresh(false);
            }
        }
        else
        {
            wifiIsConntected = false;
        }

        // Now go do something else first
        taskYIELD();
    }
}