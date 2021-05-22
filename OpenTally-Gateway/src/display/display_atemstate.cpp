#include <Arduino.h>
#include <TFT_eSPI.h>
#include "oscclient/oscclient.h"
#include "display/display_fonts.h"
#include "display/display_atemstate.h"
#include "atemswitcher/atemswitcher.h"

extern TFT_eSPI tft;
extern bool displayBlinkState;

int _lastAtemState = 255;
bool _lastAtemBlinkState = false;

void display_atemstate()
{
    int newAtemState = (atemswithcer_isconnected() ? 1 : 0);
    if(_lastAtemState != newAtemState || (!newAtemState && _lastAtemBlinkState != displayBlinkState))
    {
        uint32_t foreColor = newAtemState ? TFT_GREEN : (displayBlinkState ? TFT_RED : TFT_DARKGREY);
        //tft.loadFont(AA_FONT_SMALL);        
        tft.setTextFont(2);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(foreColor, TFT_BLACK);
        tft.fillRect(tft.width() - 100,0,20,20, TFT_BLACK);
        tft.setCursor(tft.width() - 90,2);
        tft.println("ATEM");
        //tft.unloadFont();
        _lastAtemState = newAtemState;
        _lastAtemBlinkState = displayBlinkState;
    }
}