#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

#include "state_client_channel.h"
#include "buildconfig.h"
#include "display/display.h"

#ifdef IS_OSCCLIENT
    #include "oscclient/oscclient.h"
#endif

static SemaphoreHandle_t clientChannelMutex = xSemaphoreCreateMutex();

struct ClientChannel
{
    int channel;
};

ClientChannel clientChannelConfig;

void writeClientChannel()
{
    Serial.println("[Client channel]::Writing to flash.");
    File clientChannelFile = SPIFFS.open("/clientchannel.cfg", "w");
    if(clientChannelFile)
    {
        clientChannelFile.write((byte*) &clientChannelConfig, sizeof(clientChannelConfig));
        clientChannelFile.flush();
        clientChannelFile.close();
        Serial.print("[Client channel]::");
        Serial.print(clientChannelConfig.channel);
        Serial.println(" written to flash.");
    }
    else
    {
        Serial.println("[Client channel]::Fopen failed!");
    }

    #ifdef IS_OSCCLIENT
        oscclient_request_resubscribe();
    #endif
}

void readClientChannel()
{
    clientChannelConfig.channel = 1;
    if(SPIFFS.exists("/clientchannel.cfg"))
    {
        Serial.println("[Client channel]::Reading from flash.");
        File clientChannelFile = SPIFFS.open("/clientchannel.cfg", "r");
        clientChannelFile.readBytes((char*) &clientChannelConfig, sizeof(clientChannelConfig));
        clientChannelFile.close();
        Serial.print("[Client channel]::");
        Serial.print(clientChannelConfig.channel);
        Serial.println(" read from flash.");
    }
    else 
    {
        Serial.println("[Client channel]::Config not found - initializing.");
        writeClientChannel();
    }
}

void state_clientchannel_setup()
{
    readClientChannel();
}

void state_setclientchannel(int numChannel)
{
    xSemaphoreTake(clientChannelMutex, portMAX_DELAY);
    clientChannelConfig.channel = numChannel;
    xSemaphoreGive(clientChannelMutex);  
    writeClientChannel();
    display_request_refresh(false);
}

int state_getclientchannel()
{
    return clientChannelConfig.channel;
}

int state_cycleclientchannel()
{
    xSemaphoreTake(clientChannelMutex, portMAX_DELAY);
    clientChannelConfig.channel++;
    if(clientChannelConfig.channel > 8)
        clientChannelConfig.channel = 1;
    xSemaphoreGive(clientChannelMutex);  
    writeClientChannel();
    display_request_refresh(false);

    return clientChannelConfig.channel;
}