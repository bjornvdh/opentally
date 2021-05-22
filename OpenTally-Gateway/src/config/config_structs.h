#ifndef __CONFIG_STRUCTS_H
#define __CONFIG_STRUCTS_H

#include <Arduino.h>
#include "config/config_enums.h"

struct LEDModeConfig
{
    LedMode Mode;
    int Param1;
};

struct LEDConfig
{
    int BaseColor;     // Light the LED up in a dim, solid base color
    LEDModeConfig SolidColor;
    LEDModeConfig Blink;
};

struct ConfigAction
{
    KeyAction Action;
    int Param1;
    String Param2;
};

struct KeyConfig
{
    ConfigAction OnPress;
    ConfigAction OnLongpress;
};

#endif