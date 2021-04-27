#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPIFFS.h>

#include "net/net.h"
#include "display/display_fonts.h"

extern bool displayBlinkState;
extern TFT_eSPI tft;
uint8_t currentWifiDisplayMode = 0;

#define WIFIICON_LEFT 202
#define WIFIICON_TOP  2
#define WIFIICONRECT_LEFT 200
#define WIFIICONRECT_TOP 0
#define WIFIICONRECT_WIDTH 40
#define WIFIICONRECT_HEIGHT 16

#define WIFIBAR_HEIGHT 14
#define WIFIBAR_WIDTH 6
#define WIFIBAR_SPACING 2

#define WIFIDISPLAY_CONNECTED 1
#define WIFIDISPLAY_CONFIGAP 2
#define WIFIDISPLAY_CONNECTING 3

void display_draw_wifistatus()
{   
    if(net_wifiIsConnected())
    {
        if(currentWifiDisplayMode != WIFIDISPLAY_CONNECTED)
        tft.fillRect(WIFIICONRECT_LEFT, WIFIICONRECT_TOP, WIFIICONRECT_WIDTH, WIFIICONRECT_HEIGHT, TFT_BLACK);

        int barWidthSpace = WIFIBAR_WIDTH + WIFIBAR_SPACING;

        tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 0), WIFIICONRECT_TOP + 8, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 8, TFT_WHITE);
        tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 1), WIFIICONRECT_TOP + 6, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 6, TFT_WHITE);
        tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 2), WIFIICONRECT_TOP + 4, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 4, TFT_WHITE);
        tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 3), WIFIICONRECT_TOP + 2, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 2, TFT_WHITE);
        tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 4), WIFIICONRECT_TOP + 0, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 0, TFT_WHITE);
        currentWifiDisplayMode = WIFIDISPLAY_CONNECTED;

        uint8_t wifiRSSI = net_getWifiRSSI();
        tft.fillRect(WIFIICONRECT_LEFT + (barWidthSpace * 0) + 1, WIFIICONRECT_TOP + 9, WIFIBAR_WIDTH - 2, WIFIBAR_HEIGHT - 10, (wifiRSSI > -85) ? TFT_SKYBLUE : TFT_BLACK);
        tft.fillRect(WIFIICONRECT_LEFT + (barWidthSpace * 1) + 1, WIFIICONRECT_TOP + 7, WIFIBAR_WIDTH - 2, WIFIBAR_HEIGHT - 8, (wifiRSSI > -77) ? TFT_SKYBLUE : TFT_BLACK);
        tft.fillRect(WIFIICONRECT_LEFT + (barWidthSpace * 2) + 1, WIFIICONRECT_TOP + 5, WIFIBAR_WIDTH - 2, WIFIBAR_HEIGHT - 6, (wifiRSSI > -71) ? TFT_SKYBLUE : TFT_BLACK);
        tft.fillRect(WIFIICONRECT_LEFT + (barWidthSpace * 3) + 1, WIFIICONRECT_TOP + 3, WIFIBAR_WIDTH - 2, WIFIBAR_HEIGHT - 4, (wifiRSSI > -66) ? TFT_SKYBLUE : TFT_BLACK);
        tft.fillRect(WIFIICONRECT_LEFT + (barWidthSpace * 4) + 1, WIFIICONRECT_TOP + 1, WIFIBAR_WIDTH - 2, WIFIBAR_HEIGHT - 2, (wifiRSSI > -58) ? TFT_SKYBLUE : TFT_BLACK);         
    }
    else if(net_configApIsActive())
    {
        if(currentWifiDisplayMode != WIFIDISPLAY_CONFIGAP)
        tft.fillRect(WIFIICONRECT_LEFT, WIFIICONRECT_TOP, WIFIICONRECT_WIDTH, WIFIICONRECT_HEIGHT, TFT_BLACK);

        tft.loadFont(AA_FONT_SMALL);
        tft.setTextDatum(TL_DATUM);
        tft.setCursor(WIFIICON_LEFT, WIFIICON_TOP);
        tft.setTextColor(TFT_RED);
        tft.print("AP");
        tft.unloadFont();
        currentWifiDisplayMode = WIFIDISPLAY_CONFIGAP;
    }
    else
    {
        tft.fillRect(WIFIICONRECT_LEFT, WIFIICONRECT_TOP, WIFIICONRECT_WIDTH, WIFIICONRECT_HEIGHT, TFT_BLACK);

        if(displayBlinkState)
        {
            int barWidthSpace = WIFIBAR_WIDTH + WIFIBAR_SPACING;

            tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 0), WIFIICONRECT_TOP + 8, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 8, TFT_RED);
            tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 1), WIFIICONRECT_TOP + 6, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 6, TFT_RED);
            tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 2), WIFIICONRECT_TOP + 4, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 4, TFT_RED);
            tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 3), WIFIICONRECT_TOP + 2, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 2, TFT_RED);
            tft.drawRect(WIFIICONRECT_LEFT + (barWidthSpace * 4), WIFIICONRECT_TOP + 0, WIFIBAR_WIDTH, WIFIBAR_HEIGHT - 0, TFT_RED);
        }
        currentWifiDisplayMode = WIFIDISPLAY_CONNECTING;
    }
}