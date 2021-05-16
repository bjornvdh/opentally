#include "keypadeventhandler/keypadeventhandler.h"
#include "keypadeventhandler/keypadeventdef.h"
#include "keypad/keypad.h"
#include "keypadleds/keypadleds.h"
#include "config/config.h"
#include "state/state_selectedchannel.h"
#include "state/state_programpreview.h"
#include "state/state_onair.h"
#include "sleep/sleep.h"

extern QueueHandle_t keypadEventQueue;

void keypadeventhandler_setup()
{

}

// A Param1 of 0, for many actions means that the currently selected channel should be used.
// getTheChannel returns the channel number, or gets the currently selected channel.
uint8_t getTheChannel(int numChannel)
{
    if(numChannel > 0)
        return numChannel;
    else
        return state_getselectedchannel();
}

inline bool getNewState(String param2, bool currentState)
{
    ToggleModeEnum toggleMode = (ToggleModeEnum)param2.toInt();
    switch(toggleMode)
    {
        case ToggleModeEnum::SetOff:
            return false;
        case ToggleModeEnum::SetOn:
            return true;
        case ToggleModeEnum::Toggle:
        default: 
            return !currentState;
    }
}

void process_keyaction(KeypadEventDef keypadEvent, ConfigAction actionCfg)
{
    uint8_t numChannel;
    bool currentState;
    bool newState;    
    switch(actionCfg.Action)
    {
        case KeyAction::NoAction:
            break;
        case KeyAction::DeepSleep:
            sleep_start();
            break;
        case KeyAction::SelectChannel:
            state_setselectedchannel(actionCfg.Param1, (ToggleModeEnum)actionCfg.Param2.toInt());
            break;
        case KeyAction::SetPreview:
            numChannel = getTheChannel(actionCfg.Param1);    // 0 for Param1 means the currently selected channel.
            if(numChannel > 0)  
            {
                currentState = state_getchannelpreviewstate(numChannel);
                newState = getNewState(actionCfg.Param2, currentState);
                if(newState != currentState)state_setchannelpreviewstate(numChannel, newState);
            }
            break;
        case KeyAction::SetProgram:
            numChannel = getTheChannel(actionCfg.Param1);    // 0 for Param1 means the currently selected channel.
            if(numChannel > 0)  
            {
                currentState = state_getchannelprogramstate(numChannel);
                newState = getNewState(actionCfg.Param2, currentState);
                if(newState != currentState)state_setchannelprogramstate(numChannel, newState);
            }
            break;
        case KeyAction::SetOnAirState:
            int newState = actionCfg.Param1;
            if(newState > 2)
            {
                newState = ((int)onair_getstate()) + 1;
                if(newState > 2)
                    newState = 0;                
            }
            
            onair_setstate((OnAirState) newState);
            break;            
    }
}

void keypadeventhandler_task(void* parameters)
{
    KeypadEventDef keypadEvent;
    while(true)
    {
        if(sleepInitiated)
        {
            taskYIELD();
            continue;  // Stop processing the keypad queue
        } 

        xQueueReceive(keypadEventQueue, (void*)&keypadEvent, portMAX_DELAY);

        keypadleds_request_refresh();
        
        Serial.print("Key ");
        Serial.print(keypadEvent.numKey);
        KeyConfig cfg;
        switch(keypadEvent.eventType)
        {
            case KEYPAD_KEY_PRESSED:
                Serial.println(" pressed.");
                break;
            case KEYPAD_KEY_LONGPRESSED:
                Serial.println(" long pressed.");

                cfg = config_getkeyconfig(keypadEvent.numKey);
                process_keyaction(keypadEvent, cfg.OnLongpress);

                break;
            case KEYPAD_KEY_RELEASED:
                Serial.println(" released.");

                if(keypadEvent.previousState == KEYPAD_KEY_PRESSED) // Single short press
                {
                    cfg = config_getkeyconfig(keypadEvent.numKey);
                    process_keyaction(keypadEvent, cfg.OnPress);
                }

                break;
        }
    }
}