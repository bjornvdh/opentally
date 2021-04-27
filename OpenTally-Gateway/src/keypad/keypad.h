#include <Arduino.h>
#include "keypadeventhandler/keypadeventdef.h"

#ifndef __KEYPAD_H
#define __KEYPAD_H

extern KeypadEventDef keypadEvent;
extern QueueHandle_t keypadEventQueue;
#define KEYPAD_QUEUE_LEN 10

#define KEYPAD_KEY_PRESSED 1
#define KEYPAD_KEY_RELEASED 0
#define KEYPAD_KEY_LONGPRESSED 2

uint8_t keypad_getkeystate(uint8_t numKey);
void keypad_setup();
void keypad_task(void* parameters);

#endif