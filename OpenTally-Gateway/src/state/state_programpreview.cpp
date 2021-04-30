#include <Arduino.h>
#include "state/state_programpreview.h"


bool _channelProgramState[32];
bool _channelPreviewState[32];

static SemaphoreHandle_t programPreviewStateMutex = xSemaphoreCreateMutex();

bool state_getchannelprogramstate(int numChannel)
{
    return _channelProgramState[numChannel];
}

bool state_getchannelpreviewstate(int numChannel)
{
    return _channelPreviewState[numChannel];
}

void state_setchannelprogramstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    _channelProgramState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);

    Serial.print("Channel ");
    Serial.print(numChannel);
    Serial.print(" program ");
    if(state)
        Serial.println(" ON.");
    else
        Serial.println(" off.");
}

void state_setchannelpreviewstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    _channelPreviewState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);

    Serial.print("Channel ");
    Serial.print(numChannel);
    Serial.print(" preview ");
    if(state)
        Serial.println(" ON.");
    else
        Serial.println(" off.");    
}