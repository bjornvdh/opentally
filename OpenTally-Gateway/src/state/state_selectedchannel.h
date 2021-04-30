#include <Arduino.h>

#ifndef __state_SELECTEDCHANNEL_H
#define __state_SELECTEDCHANNEL_H

#include "config/config_gateway.h"

uint8_t state_getselectedchannel();
void state_setselectedchannel(uint8_t numChannel, ToggleModeEnum toggleMode);


#endif