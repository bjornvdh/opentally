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
                    KeyConfig cfg = config_getkeyconfig(keypadEvent.numKey);
                    Serial.print("Action for this key: ");
                    Serial.print(cfg.OnPress.Action);
                    Serial.print(" - p1,p2: ");
                    Serial.print(cfg.OnPress.ActionParam1);
                    Serial.print(", ");
                    Serial.println(cfg.OnPress.ActionParam2);
                    switch(cfg.OnPress.Action)
                    {
                        case (int)KeyAction::SelectChannel:
                            Serial.println("Select new channel.");
                            uistate_setselectedchannel(cfg.OnPress.ActionParam1, (ToggleModeEnum)cfg.OnPress.ActionParam2.toInt());
                            break;
                    }
                }

                break;
        }
    }
}