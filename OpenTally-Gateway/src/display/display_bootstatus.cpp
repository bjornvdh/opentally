#include <Arduino.h>
#include <TFT_eSPI.h>

#define BOOTSTEP_LINE_HEIGHT 10

int bootStep = -1;
extern TFT_eSPI tft;

void display_bootstep(String text)
{
    bootStep++;

    tft.setTextColor(TFT_SILVER, TFT_BLACK);
    tft.setCursor(10,(bootStep * BOOTSTEP_LINE_HEIGHT));
    tft.print(text);
}

void display_bootstepresult(bool isSuccess)
{
    if(isSuccess)
    {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(160,(bootStep * BOOTSTEP_LINE_HEIGHT));
        tft.print("[OK]");
    }
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(160,(bootStep * BOOTSTEP_LINE_HEIGHT));
        tft.print("[failed]");        
    }
}