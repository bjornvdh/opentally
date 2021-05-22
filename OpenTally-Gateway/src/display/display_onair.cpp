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
    //tft.loadFont(AA_FONT_SMALL);    
    tft.setTextFont(2);
    //tft.fillRect(0,0,100,10,TFT_BLACK);
    tft.setCursor(4,2);
    tft.setTextDatum(TL_DATUM);
    tft.print("LIVE");
    //tft.unloadFont();
}

void display_state_offair()
{
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    //tft.loadFont(AA_FONT_SMALL);    
    tft.setTextFont(2);
    //tft.fillRect(0,0,100,10,TFT_BLACK);
    tft.setCursor(4,2);
    tft.setTextDatum(TL_DATUM);
    tft.print("LIVE");
    //tft.unloadFont();
    tft.drawLine(2,2,35,15,TFT_DARKGREY);
}

void display_state_countdown()
{
    int count = onair_getcount();
    if(previousCountValue != count)
    {

        previousCountValue = count;
    }
}

void display_onair()
{
    OnAirState newState = onair_getstate();
    int count = onair_getcount();
    if(newState != previousOnAirState || count != previousCountValue) 
    {
        tft.fillRect(0,0,50,18,TFT_BLACK);

        switch(newState)
        {
            case OnAirState::OffAir:
                if(newState != previousOnAirState) display_state_offair();
                break;
            case OnAirState::OnAir:
                if(newState != previousOnAirState) display_state_onair();
                break;
            case OnAirState::Countdown:
                tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                //tft.loadFont(AA_FONT_SMALL);    
                tft.setTextFont(2);
                tft.setCursor(15,2);
                tft.setTextDatum(TL_DATUM);
                tft.print(count);
                //tft.unloadFont();
                break;
        }
    }
    previousCountValue = count;
    previousOnAirState = newState;
}