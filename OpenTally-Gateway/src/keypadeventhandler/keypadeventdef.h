#include <Arduino.h>

#ifndef __KEYPADEVENTDEF_H
#define __KEYPADEVENTDEF_H

struct KeypadEventDef {
    uint8_t numKey;
    uint32_t eventTime;
    uint8_t eventType;
    uint8_t previousState;
};

#endif