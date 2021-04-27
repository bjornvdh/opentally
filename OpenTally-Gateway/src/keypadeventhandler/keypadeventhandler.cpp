#include "keypadeventhandler/keypadeventhandler.h"
#include "keypadeventhandler/keypadeventdef.h"
#include "keypad/keypad.h"
#include "leds/leds.h"
#include "config/config.h"
#include "uistate/uistate_selectedchannel.h"

extern QueueHandle_t keypadEventQueue;

void keypadeventhandler_setup()
{

}

void keypadeventhandler_task(void* parameters)
{
    KeypadEventDef keypadEvent;
    while(true)
    {
        xQueueReceive(keypadEventQueue, (void*)&keypadEvent, portMAX_DELAY);

        leds_request_refresh();
        
        Serial.print("Key ");
        Serial.print(keypadEvent.numKey);
        switch(keypadEvent.eventType)
        {
            case KEYPAD_KEY_PRESSED:
                Serial.println(" pressed.");
                break;
            case KEYPAD_KEY_LONGPRESSED:
                Serial.println(" long pressed.");
                break;
            case KEYPAD_KEY_RELEASED:
                Serial.println(" released.");

                if(keypadEvent.previousState == KEYPAD_KEY_PRESSED) // Single short press
                {
                    ConfigAction action = config_getkeypressedaction(keypadEvent.numKey);
                    switch(action.Action)
                    {
                        case ACTION_SELECT_CHANNEL:
                            uistate_setselectedchannel(action.ActionParam1);
                            break;
                    }
                }

                break;
        }
    }
}