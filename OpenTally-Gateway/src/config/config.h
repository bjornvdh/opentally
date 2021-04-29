#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>
#include "config/config_structs.h"


void config_setup();
KeyConfig config_getkeyconfig(uint8_t numKey);
LEDConfig config_getledconfig(uint8_t numLed);
extern bool configIsValid;


#endif