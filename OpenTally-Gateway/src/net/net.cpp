#include <Arduino.h>
#include "buildconfig.h"
#include "net/net.h"
#include "display/display.h"
#include "display/display_bootstatus.h"
#include "sleep/sleep.h"

#include <Preferences.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Wifi.h>
#include <WifiManager.h>
#include <ESPmDNS.h>

#define RSSI_POLL_INTERVAL 2500

WiFiManager wm;
unsigned long _last_rssi_poll = 0;

bool configApIsActive = false;
bool wifiIsConntected = false;
uint8_t wifiRSSI = 0;

String _hostname;
String _titleAndApname;

String net_getMacAddress() {
    uint8_t baseMac[6];
    // Get MAC address for WiFi station
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char baseMacChr[18] = {0};
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    return String(baseMacChr);
}

String net_getMacAddressShort() {
    uint8_t baseMac[6];
    // Get MAC address for WiFi station
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char baseMacChr[13] = {0};
    sprintf(baseMacChr, "%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    return String(baseMacChr);
}

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

    _hostname = HOSTNAME_PREFIX;
    _titleAndApname = CONFIG_TITLE;
    if(APPEND_CHIPID_TO_HOSTNAME)
    {
        String macShort = net_getMacAddressShort();
        _hostname = HOSTNAME_PREFIX + macShort;
        _titleAndApname = CONFIG_TITLE + macShort;
    }

    const char *hostnameChar = _hostname.c_str();
    const char *titleChar = _titleAndApname.c_str();
    wm.setHostname(hostnameChar);
    wm.setConfigPortalBlocking(false);
    wm.setWiFiAutoReconnect(true);
    wm.setTitle(_titleAndApname);

    if(wm.autoConnect(titleChar)){
        Serial.println("connected...yay :)");
    }
    else {
        Serial.println("Configportal running");
    } 

    display_bootstepresult(true);
    display_bootstep("Starting mDNS...");
    if(!MDNS.begin(hostnameChar)) {
        
        Serial.println("Error starting mDNS");
        display_bootstepresult(false);
        while(true) {}
    }
    else
    {
        MDNS.addService("http", "tcp", 80);
        #ifdef IS_OSCSERVER
            MDNS.addService("opentally", "udp", OPENTALLY_UDP_PORT);
        #endif
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
        vTaskDelay(1);
    }
}