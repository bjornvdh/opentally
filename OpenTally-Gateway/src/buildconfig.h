#include <Arduino.h>

#pragma once

// Select a build config. 
// This defines what the behavior of the device will be and what components will be activated.
// ONLY ONE should be enabled at any time.


#define BUILDCONFIG_GATEWAY
//#define BUILDCONFIG_DISPLAY
//#define BUILDCONFIG_TALLY

#ifdef BUILDCONFIG_GATEWAY
    #define HAS_KEYPAD_16KEYS
    #define HAS_KEYPAD_LEDS
    #define IS_OSCSERVER
    #define IS_ONAIR_CONTROLLER
    #define OPENTALLY_DEVICE_TYPE 2
#endif

#ifdef BUILDCONFIG_DISPLAY
    #define OPENTALLY_DEVICE_TYPE 1
    #define IS_OSCCLIENT
#endif


#ifdef BUILDCONFIG_TALLY
    #define OPENTALLY_DEVICE_TYPE 0
    #define IS_OSCCLIENT
#endif


