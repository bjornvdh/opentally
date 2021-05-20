#include <Arduino.h>
#include "buildconfig.h"
#include "keypad/keypad.h"
#include "keypadeventhandler/keypadeventdef.h"
#include "display/display_bootstatus.h"
#include "sleep/sleep.h"

#define GPIO_FEED_ROW1 2
#define GPIO_FEED_ROW2 17
#define GPIO_FEED_ROW3 22
#define GPIO_FEED_ROW4 21

#define GPIO_SENSE_COL1  33
#define GPIO_SENSE_COL2  25
#define GPIO_SENSE_COL3  26
#define GPIO_SENSE_COL4  27

#define KEYPAD_NUM_ROWS 4
#define KEYPAD_NUM_COLS 4

#define DEBOUNCE_TIME 10 // mSec
#define LONGPRESS_TIME 750 // mSec

int _sense_cols[] = {GPIO_SENSE_COL1, GPIO_SENSE_COL2, GPIO_SENSE_COL3, GPIO_SENSE_COL4};
int _feed_rows[] = {GPIO_FEED_ROW1, GPIO_FEED_ROW2, GPIO_FEED_ROW3, GPIO_FEED_ROW4};


uint8_t keyState[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t keyHoldOff[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

KeypadEventDef keypadEvent;
QueueHandle_t keypadEventQueue;

void keypad_setup()
{
    display_bootstep("Starting keypad...");
    #ifdef HAS_KEYPAD_16KEYS
        pinMode(GPIO_SENSE_COL1, INPUT);
        pinMode(GPIO_SENSE_COL2, INPUT);
        pinMode(GPIO_SENSE_COL3, INPUT);
        pinMode(GPIO_SENSE_COL4, INPUT);

        pinMode(GPIO_FEED_ROW1, OUTPUT);
        pinMode(GPIO_FEED_ROW2, OUTPUT);
        pinMode(GPIO_FEED_ROW3, OUTPUT);
        pinMode(GPIO_FEED_ROW4, OUTPUT);
    #endif

    pinMode(KEYPAD_HWBUTTON1, INPUT);


    keypadEventQueue = xQueueCreate(KEYPAD_QUEUE_LEN, sizeof(KeypadEventDef));
    display_bootstepresult(true);
}

uint8_t keypad_getkeystate(uint8_t numKey)
{
    return keyState[numKey];
}

inline void handleKeystate(uint32_t theTime, uint8_t numKey, bool keyIsPressed)
{
    uint8_t previousState = keyState[numKey];
    if((!keyIsPressed && keyState[numKey] > KEYPAD_KEY_RELEASED) || (keyIsPressed && keyState[numKey] == KEYPAD_KEY_RELEASED))
    {
        // The key was not pressed and is now pressed or
        // the key was pressed and is now not pressed.
        keyHoldOff[numKey] = theTime + DEBOUNCE_TIME;
        keyState[numKey] = keyIsPressed ? KEYPAD_KEY_PRESSED :  KEYPAD_KEY_RELEASED;
        keypadEvent.eventType = keyState[numKey];
        keypadEvent.eventTime = theTime;
        keypadEvent.previousState = previousState;
        keypadEvent.numKey = numKey;

        while(xQueueSend(keypadEventQueue, (void*)&keypadEvent, 500) != pdTRUE);                            
    }
    else if(keyIsPressed  && keyState[numKey] == KEYPAD_KEY_PRESSED && (theTime - keyHoldOff[numKey]) > LONGPRESS_TIME)
    {
        // The key has been pressed for a while.
        keyState[numKey] = KEYPAD_KEY_LONGPRESSED;
        keypadEvent.eventTime = theTime;
        keypadEvent.eventType = KEYPAD_KEY_LONGPRESSED;
        keypadEvent.previousState = previousState;
        keypadEvent.numKey = numKey;

        while(xQueueSend(keypadEventQueue, (void*)&keypadEvent, 500) != pdTRUE);
    }
}

void keypad_task(void* parameters)
{
    while(true)
    {
        if(sleepInitiated){
            // If we go to sleep, do nothing with the keypad.
            taskYIELD();
            continue;
        }

        uint32_t theTime = millis();
        #ifdef HAS_KEYPAD_16KEYS
            // Scan the keypad row by row
            for(uint8_t numRow = 0; numRow < KEYPAD_NUM_ROWS; numRow++)
            {
                digitalWrite(_feed_rows[numRow], HIGH);

                // And read all columns
                for(uint8_t numCol = 0; numCol < KEYPAD_NUM_COLS; numCol++)
                {
                    uint8_t numKey = (numCol) + (numRow * 4);
                    if(keyHoldOff[numKey] < theTime) // Key is not being debounced
                    {
                        bool keyIsPressed = (digitalRead(_sense_cols[numCol]) == 1);
                        handleKeystate(theTime, numKey, keyIsPressed);
                    }
                }
                digitalWrite(_feed_rows[numRow], LOW);
            }
        #endif
        // Scan the two physical buttons on the UC itself
        if(keyHoldOff[16] < theTime) // Key is not being debounced
        {
            bool keyIsPressed = KEYPAD_HWBUTTONS_ACTIVE_HIGH ? digitalRead(KEYPAD_HWBUTTON1) : !digitalRead(KEYPAD_HWBUTTON1); 
            handleKeystate(theTime, 16, keyIsPressed);
        }

        if(keyHoldOff[17] < theTime) // Key is not being debounced
        {
            bool keyIsPressed = KEYPAD_HWBUTTONS_ACTIVE_HIGH ? digitalRead(KEYPAD_HWBUTTON2) : !digitalRead(KEYPAD_HWBUTTON2); 
            handleKeystate(theTime, 17, keyIsPressed);
        }

        // Go and do something else now.
        taskYIELD();
    }
}