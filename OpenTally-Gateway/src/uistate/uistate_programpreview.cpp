#include <Arduino.h>
#include "uistate/uistate_programpreview.h"


bool _channelProgramState[32];
bool _channelPreviewState[32];

static SemaphoreHandle_t programPreviewStateMutex = xSemaphoreCreateMutex();

bool uistate_getchannelprogramstate(int numChannel)
{
    return _channelProgramState[numChannel];
}

bool uistate_getchannelpreviewstate(int numChannel)
{
    return _channelPreviewState[numChannel];
}

bool uistate_setchannelprogramstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    _channelProgramState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);
}

bool uistate_setchannelpreviewstate(int numChannel, bool state)
{
    xSemaphoreTake(programPreviewStateMutex, portMAX_DELAY);
    _channelPreviewState[numChannel] = state;
    xSemaphoreGive(programPreviewStateMutex);
}