#include <Arduino.h>

#pragma once

// Select a build config. 
// This defines what the behavior of the device will be and what components will be activated.
// ONLY ONE should be enabled at any time.


#define BUILDCONFIG_GATEWAY
//#define BUILDCONFIG_DISPLAY
//#define BUILDCONFIG_TALLY

// The hostname used by the gateway. 
// The hostname this device uses is dependent on the chosen BUILDCONFIG
#define GATEWAY_HOSTNAME "OpenTallyGW"

// The UDP port number used for communication.
#define OPENTALLY_UDP_PORT 13323

// The maximum number of supported channels, including an unused channel "0"
#define CHANNEL_COUNT 10    

// Define which buttons are the hardware buttons


#define TALLY_LEDS_COUNT 9
#define TALLY_LEDS_PIN 12

#define BUZZER_PIN 17
#define LEDPANEL_PIN 14

#ifdef BUILDCONFIG_GATEWAY
    #define KEYPAD_HWBUTTONS_ACTIVE_HIGH false
    #define KEYPAD_HWBUTTON1 35 // 35
    #define KEYPAD_HWBUTTON2 0 // 0
    #define KEYPAD_GPIO_WAKEUP_SEL GPIO_SEL_35
    #define HAS_KEYPAD_16KEYS
    #define HAS_KEYPAD_LEDS true
    #define IS_OSCSERVER
    #define IS_ONAIR_CONTROLLER
    #define OPENTALLY_DEVICE_TYPE 2
    #define HOSTNAME_PREFIX GATEWAY_HOSTNAME
    #define APPEND_CHIPID_TO_HOSTNAME false
    #define CONFIG_TITLE "OpenTally Gateway"
    #define CONFIG_FILE_NAME "/gatewaycfg.json"
#endif

#ifdef BUILDCONFIG_DISPLAY
    #define KEYPAD_HWBUTTONS_ACTIVE_HIGH true
    #define KEYPAD_HWBUTTON1 33 // 35
    #define KEYPAD_HWBUTTON2 32 // 0
    #define KEYPAD_GPIO_WAKEUP_SEL GPIO_SEL_35

    #define HAS_LEDPANEL

    #define OPENTALLY_DEVICE_TYPE 1
    #define IS_OSCCLIENT
    #define HOSTNAME_PREFIX "OTDisp"
    #define APPEND_CHIPID_TO_HOSTNAME true
    #define CONFIG_TITLE "OpenTally Display"
    #define CONFIG_FILE_NAME "/displaycfg.json"
#endif

#ifdef BUILDCONFIG_TALLY
    #define KEYPAD_HWBUTTONS_ACTIVE_HIGH true
    #define KEYPAD_HWBUTTON1 37 // 35
    #define KEYPAD_HWBUTTON2 38 // 0
    #define KEYPAD_GPIO_WAKEUP_SEL GPIO_SEL_37
    #define OPENTALLY_DEVICE_TYPE 0
    #define IS_OSCCLIENT
    #define HOSTNAME_PREFIX "OTLight"
    #define APPEND_CHIPID_TO_HOSTNAME true    
    #define CONFIG_TITLE "OpenTally Light"
    #define HAS_TALLY_LEDS true
    #define HAS_BUZZER
    #define CONFIG_FILE_NAME "/tallycfg.json"
#endif
