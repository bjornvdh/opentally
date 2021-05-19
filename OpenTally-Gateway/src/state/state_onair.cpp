#include <Arduino.h>
#include <OSCMessage.h>
#include "state/state_onair.h"
#include "display/display.h"
#include "oscserver/oscdispatcher.h"

static SemaphoreHandle_t onAirStateMutex = xSemaphoreCreateMutex();
uint8_t _countdown_count = 0;
OnAirState _onAirState = OnAirState::OffAir;
uint8_t _onair_refreshCounter = 0;

void onair_task(void* parameters)
{
    while(true)
    {
        while(_countdown_count > 0)
        {
            vTaskDelay(1000);
            xSemaphoreTake(onAirStateMutex, portMAX_DELAY);
            _countdown_count--;
            xSemaphoreGive(onAirStateMutex);
            display_request_refresh(false);
            
            if(_countdown_count == 0)
            {
                onair_setstate(OnAirState::OnAir);
            }
            else
            {
                oscdispatch_onairstate(_onAirState, _countdown_count);                
            }
        }
        vTaskDelay(5);
    }
}

void onair_refresh_task(void* parameters)
{
    while(true)
    {
        _onair_refreshCounter++;

        if(_onAirState == OnAirState::Countdown || _onair_refreshCounter >= 5)
        {
            _onair_refreshCounter = 0;
            oscdispatch_onairstate(_onAirState, _countdown_count); 
        }

        vTaskDelay(200);
    }
}

void onair_receivestate(OSCMessage &msg)
{
    Serial.println("[STATE OnAir]::Received new state");

    OnAirState newState = (OnAirState)msg.getInt(0);

    char str[3];
    msg.getString(1, str, 3);

    int newCountDownCount = String(str).toInt();

    bool stateIsDifferent = false;

    xSemaphoreTake(onAirStateMutex, portMAX_DELAY);
    stateIsDifferent = (_onAirState != newState || _countdown_count != newCountDownCount);
    _onAirState = newState;
    _countdown_count = newCountDownCount;
    xSemaphoreGive(onAirStateMutex);    

    if(stateIsDifferent) display_request_refresh(false);
}

void onair_setstate(OnAirState newState)
{
    Serial.println("New onair state!");
    xSemaphoreTake(onAirStateMutex, portMAX_DELAY);
    _onAirState = newState;
    if(newState == OnAirState::Countdown) _countdown_count = 10;
    oscdispatch_onairstate(newState, _countdown_count);
    xSemaphoreGive(onAirStateMutex);

    display_request_refresh(false);
}

OnAirState onair_getstate()
{
    return _onAirState;
}

uint8_t onair_getcount()
{
    return _countdown_count;
}