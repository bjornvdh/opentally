#include <Arduino.h>

#include "uistate/uistate_selectedchannel.h"

static SemaphoreHandle_t selectedChannelMutex = xSemaphoreCreateMutex();
uint8_t selectedChannel;

uint8_t uistate_getselectedchannel()
{
    return selectedChannel;
}

void uistate_setselectedchannel(uint8_t numChannel)
{    
    xSemaphoreTake(selectedChannelMutex, portMAX_DELAY);
    if(selectedChannel == numChannel)
        selectedChannel = 0;            // Deselect the channel if it's already selected.
    else
        selectedChannel = numChannel;
    xSemaphoreGive(selectedChannelMutex);

    Serial.print("Channel ");
    Serial.print(selectedChannel);
    Serial.print(" selected.");
}
