#include <Arduino.h>
#include <OSCMessage.h>
#include "buildconfig.h"
#include "channelmessage.h"
#include "oscserver/oscdispatcher.h"
#include <Buzzer.h>

#include "channelsound.h"

static SemaphoreHandle_t channelSoundMutex = xSemaphoreCreateMutex();

struct ChannelSoundStruct
{
    int Sound;
    uint32_t TimeoutAt;
    bool hasPlayed;
};

ChannelSoundStruct _channelsounds[CHANNEL_COUNT];

void onBuzzerStateChange(BuzzerInfo* sender) {}

Buzzer myBuzzer(BUZZER_PIN, onBuzzerStateChange, "BUZZER");

void channelsound_setup()
{
    myBuzzer.init();
    myBuzzer.buzz(8000, 10);
    delay(50);
    myBuzzer.buzz(5500, 10); 
    delay(50);
    myBuzzer.buzz(6500, 10);     
}

void channelsound_receive(OSCMessage &msg)
{
    bool hasAlreadyPlayed = false;
    int numChannel = msg.getInt(0);

    char str[10];
    msg.getString(1, str, 10);
    int sound = String(str).toInt();

    char str2[10];
    msg.getString(2, str2, 10);
    Serial.println(str2);
    uint32_t timeLeft = String(str2).toInt();

    Serial.print("[CHANNELMSG]::Sound [");
    Serial.print(sound);
    Serial.print("] chan ");
    Serial.print(numChannel);
    Serial.print(" time ");
    Serial.print(timeLeft);
    Serial.println(".");

    xSemaphoreTake(channelSoundMutex, portMAX_DELAY);
    hasAlreadyPlayed = (_channelsounds[numChannel].Sound == sound && _channelsounds[numChannel].TimeoutAt >= millis());
    _channelsounds[numChannel].Sound = sound;
    _channelsounds[numChannel].TimeoutAt = millis() + timeLeft;
    _channelsounds[numChannel].hasPlayed = true;
    xSemaphoreGive(channelSoundMutex);

    if(!hasAlreadyPlayed)
    {
        if(sound == 0)
        {
            myBuzzer.buzz(800, 10);
            vTaskDelay(50);
            myBuzzer.buzz(550, 10); 
            vTaskDelay(50);
            myBuzzer.buzz(650, 10);        
        }
        else
        {
            myBuzzer.buzz(8000, 10);
            vTaskDelay(50);
            myBuzzer.buzz(5500, 10); 
            vTaskDelay(50);
            myBuzzer.buzz(6500, 10);  
        }
    }
}

void channelsound_set(int numChannel, int sound)
{
    uint32_t duration = 250;
    uint32_t timeoutAt = millis() + duration;
    xSemaphoreTake(channelSoundMutex, portMAX_DELAY);
    _channelsounds[numChannel].Sound = sound;
    _channelsounds[numChannel].TimeoutAt = timeoutAt;
    xSemaphoreGive(channelSoundMutex);

    #ifdef IS_ONAIR_CONTROLLER
        oscdispatch_channelsound(numChannel, sound, duration);
    #endif
}

void channelsound_task(void* parameters)
{
    while(true)
    {
        uint32_t time = millis();
        uint32_t cutoffTime = time + 150;
        xSemaphoreTake(channelSoundMutex, portMAX_DELAY);
        for(int i = 0; i < (CHANNEL_COUNT - 1); i++)
        {
            #ifdef IS_ONAIR_CONTROLLER
            if(_channelsounds[i].TimeoutAt > cutoffTime)
            {
                oscdispatch_channelsound(i, _channelsounds[i].Sound, _channelsounds[i].TimeoutAt - time);
            }

            #endif
        }
        xSemaphoreGive(channelSoundMutex);

        vTaskDelay(50);
    }
}