#include "leds.h"
#include "keypad/keypad.h"
#include <FastLED.h>

#define LEDS_SLOW_BLINK_INTERVAL 750
#define LEDS_FAST_BLINK_INTERVAL 50

#define NUM_LEDS 16
#define LED_PIN 32

CRGB leds[NUM_LEDS];

uint32_t lastSlowBlinkToggle;
uint32_t lastFastBlinkToggle;

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
        leds_request_refresh();
    }

    if((theTime - lastFastBlinkToggle) > LEDS_FAST_BLINK_INTERVAL)
    {
        lastFastBlinkToggle = theTime;
        fastBlinkState = !fastBlinkState;
        leds_request_refresh();
    }    
}

void refreshSetBaseColor()
{
    for(int numLed = 0; numLed < NUM_LEDS; numLed++)
    {
        leds[numLed].setRGB(0,0,25);
    }    
}

void refreshOverrideKeypad()
{
    for(int numLed = 0; numLed < NUM_LEDS; numLed++)
    {
        int keyState = keypad_getkeystate(numLed);
        if(keyState == KEYPAD_KEY_PRESSED || (keyState == KEYPAD_KEY_LONGPRESSED && fastBlinkState))  // Pressed or longpressed
        {
            leds[numLed].setRGB(255,255,255);
        }
    }        
}

void leds_request_refresh()
{
    if(!ledsRefreshIsRequested)
    {
        xSemaphoreTake(ledsRefreshRequestMutex, portMAX_DELAY);
        ledsRefreshIsRequested = true;
        xSemaphoreGive(ledsRefreshRequestMutex);
    }
}

void leds_setup()
{
    ledsRefreshRequestMutex = xSemaphoreCreateMutex();

    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 
    refreshSetBaseColor();
    FastLED.show();    
}

void leds_task(void* parameters)
{
    while(true)
    {
        updateBlink();

        if(ledsRefreshIsRequested)
        {
            refreshSetBaseColor();
            refreshOverrideKeypad();
            FastLED.show();
        }

        // Once per 10ms is more than enough
        vTaskDelay(10);
    }
}

