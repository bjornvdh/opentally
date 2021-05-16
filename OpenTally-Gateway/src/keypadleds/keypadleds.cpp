#include "keypadleds.h"
#include "keypad/keypad.h"
#include "config/config.h"
#include "state/state_selectedchannel.h"
#include "state/state_programpreview.h"
#include "sleep/sleep.h"

#include <FastLED.h>

#define LEDS_SLOW_BLINK_INTERVAL 500
#define LEDS_FAST_BLINK_INTERVAL 100

#define NUM_LEDS 16
#define LED_PIN 32

CRGB leds[NUM_LEDS];

const uint8_t COLOR_BASECOLOR[3] = {0,0,25};
const uint8_t COLOR_SELECTED_CHANNEL[3] = {255,200,0};
const uint8_t COLOR_PREVIEW[3] = {0,180,0};
const uint8_t COLOR_PROGRAM[3] = {255,0,0};

uint32_t lastSlowBlinkToggle;
uint32_t lastFastBlinkToggle;
uint8_t _selectedChannel;

bool ledsRefreshIsRequested = false;
bool slowBlinkState = false;
bool fastBlinkState = false;

static SemaphoreHandle_t ledsRefreshRequestMutex;

void updateBlink()
{
    uint32_t theTime = millis();
    if((theTime - lastSlowBlinkToggle) > LEDS_SLOW_BLINK_INTERVAL)
    {
        lastSlowBlinkToggle = theTime;
        slowBlinkState = !slowBlinkState;
        keypadleds_request_refresh();
    }

    if((theTime - lastFastBlinkToggle) > LEDS_FAST_BLINK_INTERVAL)
    {
        lastFastBlinkToggle = theTime;
        fastBlinkState = !fastBlinkState;
        keypadleds_request_refresh();
    }    
}

inline void setLedColor(int numLed, const uint8_t ledColor[3])
{
    leds[numLed][0] = ledColor[0];
    leds[numLed][1] = ledColor[1];
    leds[numLed][2] = ledColor[2];
}

void setModeBasedLedColor(int numLed, LEDModeConfig modeConfig)
{
    switch(modeConfig.Mode)
    {
        case LedMode::None:
            break;
        case LedMode::SelectedChannel: 
            if(modeConfig.Param1 == _selectedChannel) setLedColor(numLed, COLOR_SELECTED_CHANNEL);
            break;            
        case LedMode::ProgramPreview:
            bool previewState = state_getchannelpreviewstate(modeConfig.Param1);
            bool programState = state_getchannelprogramstate(modeConfig.Param1);

            if(programState)
                setLedColor(numLed, COLOR_PROGRAM);
            else if(previewState)
                setLedColor(numLed, COLOR_PREVIEW);

            break;

    }
}

inline void refreshSetBaseColor(int numLed, LEDConfig ledConfig)
{
    if(ledConfig.BaseColor)
        leds[numLed].setRGB(0,0,25);
    else
        leds[numLed].setRGB(0,0,0);
}

inline void refreshOverrideKeypad(int numLed)
{
    int keyState = keypad_getkeystate(numLed);
    if(keyState == KEYPAD_KEY_PRESSED || (keyState == KEYPAD_KEY_LONGPRESSED && fastBlinkState))  // Pressed or longpressed
    {
        leds[numLed].setRGB(255,255,255);
    }
}

void keypadleds_request_refresh()
{
    if(!ledsRefreshIsRequested)
    {
        xSemaphoreTake(ledsRefreshRequestMutex, portMAX_DELAY);
        ledsRefreshIsRequested = true;
        xSemaphoreGive(ledsRefreshRequestMutex);
    }
}

void keypadleds_setup()
{
    ledsRefreshRequestMutex = xSemaphoreCreateMutex();

    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 
    //refreshSetBaseColor();
    FastLED.show();    
}

void keypadleds_task(void* parameters)
{
    while(true)
    {
        if(sleepInitiated)
        {
            // If we go to sleep, turn off all leds and do nothing more.
            FastLED.clear(true);
            taskYIELD();
            continue;  
        } 

        updateBlink();
        _selectedChannel = state_getselectedchannel();
        if(ledsRefreshIsRequested)
        {
            for(int numLed = 0; numLed < 16; numLed++)
            {
                LEDConfig config = config_getledconfig(numLed);
                refreshSetBaseColor(numLed, config);
                setModeBasedLedColor(numLed, config.SolidColor);
                if(slowBlinkState) setModeBasedLedColor(numLed, config.Blink);
                refreshOverrideKeypad(numLed);
            }

            FastLED.show();

            xSemaphoreTake(ledsRefreshRequestMutex, portMAX_DELAY);
            ledsRefreshIsRequested = false;
            xSemaphoreGive(ledsRefreshRequestMutex);

            // Wait a bit longer if we just refreshed.
            vTaskDelay(25);      
        }
        // Update every 10ms is more than enough
        vTaskDelay(10);

    }
}

