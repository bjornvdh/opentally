#include <Arduino.h>
#include "keypad/keypad.h"

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
#define LONGPRESS_TIME 500 // mSec

int _sense_cols[] = {GPIO_SENSE_COL1, GPIO_SENSE_COL2, GPIO_SENSE_COL3, GPIO_SENSE_COL4};
int _feed_rows[] = {GPIO_FEED_ROW1, GPIO_FEED_ROW2, GPIO_FEED_ROW3, GPIO_FEED_ROW4};

bool buttonIsPressed[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool buttonStateChanged[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool buttonLongPressedSent[16] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
unsigned long buttonHoldOff[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

keypadEventDef keypadEvent;
QueueHandle_t keypadQueue;

void keypad_setup()
{
    pinMode(GPIO_SENSE_COL1, INPUT_PULLDOWN);
    pinMode(GPIO_SENSE_COL2, INPUT_PULLDOWN);
    pinMode(GPIO_SENSE_COL3, INPUT_PULLDOWN);
    pinMode(GPIO_SENSE_COL4, INPUT_PULLDOWN);

    pinMode(GPIO_FEED_ROW1, OUTPUT);
    pinMode(GPIO_FEED_ROW2, OUTPUT);
    pinMode(GPIO_FEED_ROW3, OUTPUT);
    pinMode(GPIO_FEED_ROW4, OUTPUT);

    keypadQueue = xQueueCreate(KEYPAD_QUEUE_LEN, sizeof(keypadEventDef));
}

void keypad_task(void* parameters)
{
    while(true)
    {
        // Scan the keypad row by row
        for(uint8_t numRow = 0; numRow < KEYPAD_NUM_ROWS; numRow++)
        {
            uint32_t theTime = millis();
            digitalWrite(_feed_rows[numRow], HIGH);
            for(uint8_t numCol = 0; numCol < KEYPAD_NUM_COLS; numCol++)
            {
                uint8_t numButton = (numCol) + (numRow * 4);
                if(buttonHoldOff[numButton] < theTime)
                {
                    bool buttonState = digitalRead(_sense_cols[numCol]);
                    if(buttonState != buttonIsPressed[numButton])
                    {
                        buttonHoldOff[numButton] = theTime + DEBOUNCE_TIME;
                        buttonStateChanged [numButton] = true;
                        buttonIsPressed[numButton] = buttonState;
                        buttonLongPressedSent[numButton] = false;
                        keypadEvent.eventType = buttonState ? KEYPAD_EVENT_PRESSED :  KEYPAD_EVENT_RELEASED;
                        keypadEvent.eventTime = theTime;
                        keypadEvent.numKey = numButton;

                        while(xQueueSend(keypadQueue, (void*)&keypadEvent, 500) != pdTRUE);                            
                    }
                    else if(buttonState == true && !buttonLongPressedSent[numButton] && (theTime - buttonHoldOff[numButton]) > LONGPRESS_TIME)
                    {
                        buttonLongPressedSent[numButton] = true;
                        keypadEvent.eventTime = theTime;
                        keypadEvent.eventType = KEYPAD_EVENT_LONGPRESSED;
                        keypadEvent.numKey = numButton;

                        while(xQueueSend(keypadQueue, (void*)&keypadEvent, 500) != pdTRUE);
                    }
                }
            }
            digitalWrite(_feed_rows[numRow], LOW);
        }

        // Go and do something else now.
        taskYIELD();
    }
}