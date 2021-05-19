#include <Arduino.h>
#include <OSCMessage.h>

#include "state/state_onair_enums.h"

#ifndef __state_ONAIR_H
#define __state_ONAIR_H


OnAirState onair_getstate();
void onair_setstate(OnAirState state);
uint8_t onair_getcount();
void onair_task(void* parameters);
void onair_receivestate(OSCMessage &msg);
void onair_refresh_task(void* parameters);

#endif