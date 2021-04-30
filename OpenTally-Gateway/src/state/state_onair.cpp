#include <Arduino.h>

#include "state/state_onair.h"
#include "display/display.h"
#include "oscserver/oscdispatcher.h"

static SemaphoreHandle_t onAirStateMutex = xSemaphoreCreateMutex();
uint8_t _countdown_count = 0;
OnAirState _onAirState = OnAirState::OffAir;

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