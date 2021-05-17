#include <Arduino.h>
#include "display/display.h"
#include "display/display_fonts.h"
#include "display/display_draw_generic.h"
#include "display/display_bootstatus.h"
#include "display/display_onair.h"
#include "sleep/sleep.h"

#include <FS.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>

#define DISPLAY_BLINK_INTERVAL 500

TFT_eSPI tft = TFT_eSPI(135, 240);

unsigned long _last_wifi_refresh = 0;
bool displayRefreshIsRequested = false;
bool clearScreenOnRefresh = true;
bool displayBlinkState = false;
uint32_t lastDisplayBlinkToggle = 0;

static SemaphoreHandle_t displayRefreshRequestMutex;

void display_request_refresh(bool clearScreen = false)
{
    if(!displayRefreshIsRequested || (clearScreen && !clearScreenOnRefresh))
    {
        xSemaphoreTake(displayRefreshRequestMutex, portMAX_DELAY);
        displayRefreshIsRequested = true;
        if(clearScreen) clearScreenOnRefresh = true;
        xSemaphoreGive(displayRefreshRequestMutex);
    } 
}

void display_setup()
{
    displayRefreshRequestMutex = xSemaphoreCreateMutex();

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);
    display_bootstep("Starting FS...");

    if (!SPIFFS.begin()) {
        display_bootstepresult(false);       
        Serial.println("FS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    else
    {
        display_bootstepresult(true);
    }
    Serial.println("\r\nFS available!");

    display_bootstep("Detecting fonts...");
    // ESP32 will crash if any of the fonts are missing
    bool font_missing = false;
    if (SPIFFS.exists("/NotoSansBold15.vlw")    == false) font_missing = true;
    if (SPIFFS.exists("/NotoSansBold36.vlw")    == false) font_missing = true;
    if (SPIFFS.exists("/LeelawadeeUI-Bold-48.vlw")    == false) font_missing = true;

    if (font_missing)
    {
        display_bootstepresult(false);

        Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
        while(1) yield();
    }
    else 
    {
        display_bootstepresult(true);
        Serial.println("\r\nFonts found OK.");
    }
}

void display_task(void* parameters)
{
    while(true)
    {
        if(sleepInitiated)
        {
            // If we go to sleep, do nothing with the display.
            taskYIELD();
            continue;
        } 

        uint32_t theTime = millis();

        if(theTime - lastDisplayBlinkToggle > DISPLAY_BLINK_INTERVAL)
        {
            lastDisplayBlinkToggle = theTime;
            displayBlinkState = !displayBlinkState;
            display_request_refresh(false);
        }

        if(displayRefreshIsRequested)
        {
            if(clearScreenOnRefresh) tft.fillScreen(TFT_BLACK);

            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setCursor(10,55);

            display_draw_wifistatus();
            display_onair();

            xSemaphoreTake(displayRefreshRequestMutex, portMAX_DELAY);
            displayRefreshIsRequested = false;
            clearScreenOnRefresh = false;
            xSemaphoreGive(displayRefreshRequestMutex);   

            vTaskDelay(10);         
        }

        taskYIELD();
    }
}