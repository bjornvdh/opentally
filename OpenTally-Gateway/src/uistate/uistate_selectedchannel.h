#include <Arduino.h>

#ifndef __UISTATE_SELECTEDCHANNEL_H
#define __UISTATE_SELECTEDCHANNEL_H

#include "config/config.h"

uint8_t uistate_getselectedchannel();
void uistate_setselectedchannel(uint8_t numChannel, ToggleModeEnum toggleMode);


#endif