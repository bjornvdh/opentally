#include <Arduino.h>
#include "config/config_gateway.h"
#include "state/state_selectedchannel.h"

static SemaphoreHandle_t selectedChannelMutex = xSemaphoreCreateMutex();
uint8_t selectedChannel;

uint8_t state_getselectedchannel()
{
    return selectedChannel;
}

void state_setselectedchannel(uint8_t numChannel, ToggleModeEnum toggleMode)
{   
    // Serial.print("Selecting new channel: ");
    // Serial.print(numChannel);
    // Serial.print(" with togglemode ");
    // Serial.println((int)toggleMode);

    xSemaphoreTake(selectedChannelMutex, portMAX_DELAY);
    if(selectedChannel == numChannel && (toggleMode == ToggleModeEnum::SetOff || toggleMode == ToggleModeEnum::Toggle))
        selectedChannel = 0;            // Deselect the channel if it's already selected.
    else if(toggleMode == ToggleModeEnum::SetOn || toggleMode == ToggleModeEnum::Toggle)
        selectedChannel = numChannel;
    xSemaphoreGive(selectedChannelMutex);

    Serial.print("Channel ");
    Serial.print(selectedChannel);
    Serial.println(" selected.");
}
