#include <Arduino.h>
#include <TFT_eSPI.h>
#include "oscclient/oscclient.h"
#include "display/display_fonts.h"
#include "display_tallystatus.h"

extern TFT_eSPI tft;
extern bool displayBlinkState;

int _lastGatewayState = 255;
bool _lastBlinkState = false;

void display_gatewaystate()
{
    int newGatewayState = (oscclient_server_is_known() ? 1 : 0);
    if(_lastGatewayState != newGatewayState || (!newGatewayState && _lastBlinkState != displayBlinkState))
    {
        uint32_t foreColor = newGatewayState ? TFT_GREEN : (displayBlinkState ? TFT_RED : TFT_DARKGREY);
        //tft.loadFont(AA_FONT_SMALL);        
        tft.setTextFont(2);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(foreColor, TFT_BLACK);
        tft.fillRect(tft.width() - 100,0,20,20, TFT_BLACK);
        tft.setCursor(tft.width() - 90,2);
        tft.println("GW");
        //tft.unloadFont();
        _lastGatewayState = newGatewayState;
        _lastBlinkState = displayBlinkState;
    }
}