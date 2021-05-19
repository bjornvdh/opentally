#include <Arduino.h>
#include <OSCMessage.h>
#include "state/state_programpreview.h"
#include "oscserver/oscdispatcher.h"
#include "display/display.h"

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

void tallystate_refresh_task(void* parameters)
{
    int numChannel = 0;
    while(true)
    {
        numChannel++;
        if(numChannel > 31) numChannel = 0;
        xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
        bool previewState = _channelPreviewState[numChannel];
        bool programState = _channelProgramState[numChannel];
        xSemaphoreGive(programPreviewStateMutex);

        oscdispatch_tallystate(numChannel, previewState, programState);

        vTaskDelay(20);
    }
}

void state_receivetallystate(OSCMessage &msg)
{
    Serial.println("[STATE ProgramPreview]::Received new state");

    int numChannel = msg.getInt(0);

    char str[6];
    msg.getString(1, str, 6);

    bool previewState = (str[0] == '1');
    bool programState = (str[2] == '1');
    bool stateIsDifferent = false;

    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    stateIsDifferent = (_channelPreviewState[numChannel] != previewState || _channelProgramState[numChannel] != programState);
    _channelPreviewState[numChannel] = previewState;
    _channelProgramState[numChannel] = programState;
    xSemaphoreGive(programPreviewStateMutex);  

    if(stateIsDifferent) display_request_refresh(false);    
}

void state_setchannelprogramstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    bool previewState = _channelPreviewState[numChannel];
    _channelProgramState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);

    Serial.print("Channel ");
    Serial.print(numChannel);
    Serial.print(" program ");
    if(state)
        Serial.println(" ON.");
    else
        Serial.println(" off.");

    oscdispatch_tallystate(numChannel, previewState, state);
}

void state_setchannelpreviewstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    bool programState = _channelProgramState[numChannel];
    _channelPreviewState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);

    Serial.print("Channel ");
    Serial.print(numChannel);
    Serial.print(" preview ");
    if(state)
        Serial.println(" ON.");
    else
        Serial.println(" off.");

    oscdispatch_tallystate(numChannel, state, programState);
}