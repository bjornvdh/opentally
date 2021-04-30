#include <Arduino.h>
#include <TFT_eSPI.h>
#include "state/state_onair.h"
#include "display/display_fonts.h"

extern TFT_eSPI tft;
extern bool displayBlinkState;
OnAirState previousOnAirState = OnAirState::OnAir;
int previousCountValue = 0;

void display_state_onair()
{
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.loadFont(AA_FONT_SMALL);    
    //tft.fillRect(0,0,100,10,TFT_BLACK);
    tft.setCursor(2,2);
    tft.setTextDatum(TL_DATUM);
    tft.print("LIVE");
    tft.unloadFont();
}

void display_state_offair()
{
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.loadFont(AA_FONT_SMALL);    
    //tft.fillRect(0,0,100,10,TFT_BLACK);
    tft.setCursor(2,2);
    tft.setTextDatum(TL_DATUM);
    tft.print("LIVE");
    tft.unloadFont();
}

void display_state_countdown()
{
    int count = onair_getcount();
    if(previousCountValue != count)
    {
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.loadFont(AA_FONT_SMALL);    
        tft.fillRect(0,0,50,15,TFT_BLACK);
        tft.setCursor(15,2);
        tft.setTextDatum(TL_DATUM);
        tft.print(count);
        tft.unloadFont();
        previousCountValue = count;
    }
}

void display_onair()
{
    OnAirState newState = onair_getstate();
    if(newState != previousOnAirState) tft.fillRect(0,0,50,15,TFT_BLACK);

    switch(newState)
    {
        case OnAirState::OffAir:
            if(newState != previousOnAirState) display_state_offair();
            break;
        case OnAirState::OnAir:
            if(newState != previousOnAirState) display_state_onair();
            break;
        case OnAirState::Countdown:
            display_state_countdown();
            break;
    }

    previousOnAirState = newState;
}