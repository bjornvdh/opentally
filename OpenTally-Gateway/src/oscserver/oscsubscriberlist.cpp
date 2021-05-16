#include <Arduino.h>

#include "oscserver/oscsubscriberlist.h"

#define OSC_MAX_SUBSCRIBER_COUNT 64
#define SUBSCRIBER_VALID_TIME  60000

static SemaphoreHandle_t subscriberMutex = xSemaphoreCreateMutex();
OSCSubscriber _subscribers[OSC_MAX_SUBSCRIBER_COUNT];

void subscriber_addOrRefresh(IPAddress remoteIp, OSCDeviceType deviceType, uint8_t tallyChannel)
{
    uint8_t iOpenSlot = OSC_MAX_SUBSCRIBER_COUNT + 1;
    uint8_t iSubscriberSlot = OSC_MAX_SUBSCRIBER_COUNT + 1;
    uint32_t time = millis();

    xSemaphoreTake(subscriberMutex, portMAX_DELAY);
    for(uint8_t i = 0; i < OSC_MAX_SUBSCRIBER_COUNT; i++)
    {
        if(iOpenSlot > OSC_MAX_SUBSCRIBER_COUNT && _subscribers[i].ValidUntil < time)
        {
            // This slot is unused at the moment.
            iOpenSlot = i;
        }
        if(_subscribers[i].RemoteIP == remoteIp)
        {
            // We already know this subscriber!
            iSubscriberSlot = i;
            break;
        }
    }

    if(iSubscriberSlot > OSC_MAX_SUBSCRIBER_COUNT  && iOpenSlot < OSC_MAX_SUBSCRIBER_COUNT)
    {
        iSubscriberSlot = iOpenSlot;
    }

    if(iSubscriberSlot < OSC_MAX_SUBSCRIBER_COUNT)
    {
        Serial.print("Storing subscriber in slot ");
        Serial.print(iSubscriberSlot);
        Serial.println(".");
        _subscribers[iSubscriberSlot].RemoteIP = remoteIp;
        _subscribers[iSubscriberSlot].DeviceType = deviceType;
        _subscribers[iSubscriberSlot].TallyChannel = tallyChannel;
        _subscribers[iSubscriberSlot].ValidUntil = time + SUBSCRIBER_VALID_TIME;
    }
    else
    {
        Serial.println("No open slot.");
    }  
    xSemaphoreGive(subscriberMutex);
}