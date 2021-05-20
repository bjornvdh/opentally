#include <Arduino.h>
#include <FastLED.h>

#include "buildconfig.h"
#include "tallyleds.h"
#include "state/state_client_channel.h"
#include "state/state_programpreview.h"

CRGB _tallyLeds[TALLY_LEDS_COUNT];

void tallyleds_setup()
{
    FastLED.addLeds<WS2812B, TALLY_LEDS_PIN, GRB>(_tallyLeds, TALLY_LEDS_COUNT);
    FastLED.clear();
    FastLED.show(); 
}

void tallyleds_update()
{
    #ifdef HAS_TALLY_LEDS
        int numChannel = state_getclientchannel();
        bool program = state_getchannelprogramstate(numChannel);

        Serial.print("[TALLYLEDS]::Updating tally leds to ");
        Serial.println(program);

        fill_solid(_tallyLeds, TALLY_LEDS_COUNT, program ? CRGB(64,0,0) : CRGB::Black);
        FastLED.show();
    #endif
}

void tallyleds_sleep()
{
    #ifdef HAS_TALLY_LEDS
        FastLED.clear();
        FastLED.show();
    #endif
}