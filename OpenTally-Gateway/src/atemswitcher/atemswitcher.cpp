#include <Arduino.h>
#include <ATEMbase.h>
#include <ATEMstd.h>
#include <Wifi.h>
#include <WifiUdp.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Wifi.h>

#include "display/display.h"
#include "display/display_bootstatus.h"
#include "net/net.h"
#include "buildconfig.h"
#include "state/state_programpreview.h"

struct AtemSwitcherCfg
{
    IPAddress IpAddress;
    bool Enabled;
};

AtemSwitcherCfg atemSwitcherConfig;
bool _atem_connect_started = false;
bool _atem_switcher_is_connected = false;

ATEMstd atemSwitcher;
static SemaphoreHandle_t atemSwitcherMutex = xSemaphoreCreateMutex();

void writeAtemSwitcherConfig()
{
    Serial.println("[Atem Switcher]::Writing config to flash.");
    File clientChannelFile = SPIFFS.open("/atemswitcher.cfg", "w");
    if(clientChannelFile)
    {
        clientChannelFile.write((byte*) &atemSwitcherConfig, sizeof(atemSwitcherConfig));
        clientChannelFile.flush();
        clientChannelFile.close();
        Serial.print("[Atem Switcher]::");
        Serial.print(atemSwitcherConfig.IpAddress);
        Serial.println(" written to flash.");
    }
    else
    {
        Serial.println("[Atem Switcher]::Fopen failed!");
    }
}

void readAtemSwitcherConfig()
{
    atemSwitcherConfig.IpAddress = IPAddress(192,168,0,11);
    atemSwitcherConfig.Enabled = true;
    if(SPIFFS.exists("/atemswitcher.cfg"))
    {
        Serial.println("[Atem Switcher]::Reading from flash.");
        File clientChannelFile = SPIFFS.open("/atemswitcher.cfg", "r");
        clientChannelFile.readBytes((char*) &atemSwitcherConfig, sizeof(atemSwitcherConfig));
        clientChannelFile.close();
        Serial.print("[Atem Switcher]::");
        Serial.print(atemSwitcherConfig.IpAddress);
        Serial.println(" read from flash.");
    }
    else 
    {
        Serial.println("[Atem Switcher]::Config not found - initializing.");
        writeAtemSwitcherConfig();
    }
}


void atemswitcher_setconnected(bool isConnected)
{
    if(_atem_switcher_is_connected != isConnected)
    {
        xSemaphoreTake(atemSwitcherMutex, portMAX_DELAY);
        _atem_switcher_is_connected = isConnected;
        xSemaphoreGive(atemSwitcherMutex);

        display_request_refresh(false);
    }
}

void handleAtemSwitcherConnection()
{
    bool wifiIsConnected = net_wifiIsConnected();
    bool switcherIsConnected = atemSwitcher.isConnected();
    
    if(wifiIsConnected && !switcherIsConnected)
    {
        try
        {
            Serial.println("[Atem Switcher]:: Atem connect attempt.");
            atemSwitcher.connect(); 
            switcherIsConnected = atemSwitcher.isConnected();
            if(!switcherIsConnected)
                vTaskDelay(500);
        }
        catch(const std::exception& e)
        {
            Serial.println(e.what());
            vTaskDelay(1000);
        }
    }

    if(switcherIsConnected != _atem_switcher_is_connected)
    {
        atemswitcher_setconnected(switcherIsConnected);
    }
}


bool atemswitcher_isenabled()
{
    return atemSwitcherConfig.Enabled;
}

bool atemswithcer_isconnected()
{
    return _atem_switcher_is_connected;
}

void atemswitcher_setup()
{
    display_bootstep("Initializing ATEM switcher...");

    readAtemSwitcherConfig();
    atemSwitcher.begin(atemSwitcherConfig.IpAddress);

    display_bootstepresult(true);
}

void atemswitcher_task(void* params)
{
    while(true)
    {
        handleAtemSwitcherConnection();
        atemSwitcher.runLoop();

        if(atemSwitcher.isConnected())
        {
            for(int i = 1; i < CHANNEL_COUNT; i++)
            {
                bool newPreview = atemSwitcher.getPreviewTally(i);
                bool newProgram = atemSwitcher.getProgramTally(i);


                if(newPreview != state_getchannelpreviewstate(i) || newProgram != state_getchannelprogramstate(i))
                {
                    state_setchannelprogrampreviewstate(i, newPreview, newProgram);
                }
            }
        }
        taskYIELD();
    }
}