#include <Arduino.h>

#ifndef __KEYPAD_H
#define __KEYPAD_H

struct keypadEventDef {
    uint8_t numKey;
    uint32_t eventTime;
    uint8_t eventType;
};

extern keypadEventDef keypadEvent;
extern QueueHandle_t keypadQueue;
#define KEYPAD_QUEUE_LEN 10

#define KEYPAD_EVENT_PRESSED 1
#define KEYPAD_EVENT_RELEASED 0
#define KEYPAD_EVENT_LONGPRESSED 2

void keypad_setup();
void keypad_task(void* parameters);

#endif