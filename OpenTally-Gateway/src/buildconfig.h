#include <Arduino.h>

#pragma once

// Select a build config. 
// This defines what the behavior of the device will be and what components will be activated.
// ONLY ONE should be enabled at any time.


//#define BUILDCONFIG_GATEWAY
//#define BUILDCONFIG_DISPLAY
#define BUILDCONFIG_TALLY

#define GATEWAY_HOSTNAME "OpenTallyGW"
#define OPENTALLY_UDP_PORT 13323

#ifdef BUILDCONFIG_GATEWAY
    #define HAS_KEYPAD_16KEYS
    #define HAS_KEYPAD_LEDS
    #define IS_OSCSERVER
    #define IS_ONAIR_CONTROLLER
    #define OPENTALLY_DEVICE_TYPE 2
    #define HOSTNAME_PREFIX GATEWAY_HOSTNAME
    #define APPEND_CHIPID_TO_HOSTNAME false
    #define CONFIG_TITLE "OpenTally Gateway"
#endif

#ifdef BUILDCONFIG_DISPLAY
    #define OPENTALLY_DEVICE_TYPE 1
    #define IS_OSCCLIENT
    #define HOSTNAME_PREFIX "OTDisp"
    #define APPEND_CHIPID_TO_HOSTNAME true
    #define CONFIG_TITLE "OpenTally Display"
#endif

#ifdef BUILDCONFIG_TALLY
    #define OPENTALLY_DEVICE_TYPE 0
    #define IS_OSCCLIENT
    #define HOSTNAME_PREFIX "OTLight"
    #define APPEND_CHIPID_TO_HOSTNAME true    
    #define CONFIG_TITLE "OpenTally Light"
#endif
