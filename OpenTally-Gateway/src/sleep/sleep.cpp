#include <Arduino.h>
#include "sleep/sleep.h"
#include <TFT_eSPI.h> 

bool sleepInitiated;
extern TFT_eSPI tft;


void sleep_start()
{
    sleepInitiated = true;
    int r = digitalRead(TFT_BL);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Switching off...",  tft.width() / 2, tft.height() / 2 );

    vTaskDelay(5000);
    digitalWrite(TFT_BL, !r);

    tft.writecommand(TFT_DISPOFF);
    tft.writecommand(TFT_SLPIN);

    // Wait until the key is released
    while(!digitalRead(35))
    {
        vTaskDelay(10);
    }
    // Wait a bit longer for debounce.
    vTaskDelay(10);

    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();  
}