#include <Arduino.h>
#include <OSCMessage.h>
#include "buildconfig.h"
#include "channelmessage.h"
#include "display/display.h"
#include "oscserver/oscdispatcher.h"

static SemaphoreHandle_t channelMessageMutex = xSemaphoreCreateMutex();

struct ChannelMessageStruct
{
    String Message;
    uint32_t TimeoutAt;
};

ChannelMessageStruct _messages[CHANNEL_COUNT];

void channelmessage_receive(OSCMessage &msg)
{
    int numChannel = msg.getInt(0);

    char str[256];
    msg.getString(1, str, 256);
    String message = String(str);

    char str2[10];
    msg.getString(2, str2, 10);
    Serial.println(str2);
    uint32_t timeLeft = String(str2).toInt();

    Serial.print("[CHANNELMSG]::Message [");
    Serial.print(message);
    Serial.print("] chan ");
    Serial.print(numChannel);
    Serial.print(" time ");
    Serial.print(timeLeft);
    Serial.println(".");

    xSemaphoreTake(channelMessageMutex, portMAX_DELAY);
    _messages[numChannel].Message = message;
    _messages[numChannel].TimeoutAt = millis() + timeLeft;
    xSemaphoreGive(channelMessageMutex);

    display_request_refresh(false);
}

void channelmessage_set(int numChannel, String message, uint32_t duration)
{
    uint32_t timeoutAt = millis() + duration;
    xSemaphoreTake(channelMessageMutex, portMAX_DELAY);
    _messages[numChannel].Message = message;
    _messages[numChannel].TimeoutAt = timeoutAt;
    xSemaphoreGive(channelMessageMutex);

    #ifdef IS_ONAIR_CONTROLLER
        oscdispatch_channelmessage(numChannel, message, duration);
    #endif

    display_request_refresh(false);
}

void channelmessage_task(void* parameters)
{
    while(true)
    {
        uint32_t time = millis();
        uint32_t cutoffTime = time + 500;
        xSemaphoreTake(channelMessageMutex, portMAX_DELAY);
        for(int i = 0; i < (CHANNEL_COUNT - 1); i++)
        {
            #ifdef IS_ONAIR_CONTROLLER
            if(!_messages[i].Message.isEmpty() && _messages[i].TimeoutAt > cutoffTime)
            {
                oscdispatch_channelmessage(i, _messages[i].Message, _messages[i].TimeoutAt - time);
            }
            #endif

            if(!_messages[i].Message.isEmpty() && _messages[i].TimeoutAt < time)
            {
                _messages[i].Message = "";
                _messages[i].TimeoutAt = 0;
                display_request_refresh(false);
            }
        }
        xSemaphoreGive(channelMessageMutex);

        vTaskDelay(500);
    }
}

String channelmessage_getall()
{
    String retVal = "";
    uint32_t time = millis();

    xSemaphoreTake(channelMessageMutex, portMAX_DELAY);
    for(int i = 0; i < (CHANNEL_COUNT - 1); i++)
    {
        if(!_messages[i].Message.isEmpty() && _messages[i].TimeoutAt > time)
        {
            char str[260];
            sprintf(str, "[%u] %s\n", i, _messages[i].Message.c_str());
            retVal += String(str);
        }
    }
    xSemaphoreGive(channelMessageMutex);

    return retVal;
}

String channelmessage_get(int numChannel)
{
    String retVal;
    uint32_t time = millis();

    xSemaphoreTake(channelMessageMutex, portMAX_DELAY);

    if(!_messages[numChannel].Message.isEmpty() && _messages[numChannel].TimeoutAt > time)
    {
        retVal = _messages[numChannel].Message;
    }

    xSemaphoreGive(channelMessageMutex);

    return retVal;
}