#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "net/net.h"
#include "state/state_onair.h"
#include "oscclient/oscclient.h"
//#include "OpenSansCondensedBold8.h"

#include "buildconfig.h"

Adafruit_NeoMatrix ledMatrix = Adafruit_NeoMatrix(32, 8, LEDPANEL_PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG ,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  ledMatrix.Color(255, 0, 0), ledMatrix.Color(0, 255, 0), ledMatrix.Color(0, 0, 255) };


int x    = ledMatrix.width();
int pass = 0;
const uint16_t _color_red = ledMatrix.Color(255,0,0);
const uint16_t _color_yellow = ledMatrix.Color(255,255,0);
const uint16_t _color_green = ledMatrix.Color(0,255,0);
const uint16_t _color_blue = ledMatrix.Color(0,0,255);
const uint16_t _color_white = ledMatrix.Color(128,128,128);
int blinkCounter = 0;
bool ledPanelBlinkState = false;

void displayCenteredText(String text, uint16_t color)
{
    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;

    ledMatrix.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);


    ledMatrix.setCursor((ledMatrix.width() / 2) - (w / 2), (ledMatrix.height() / 2) - (int16_t)(h/2));
    ledMatrix.setTextColor(color);
    ledMatrix.println(text);

}

void ledpanel_setup()
{
    Serial.println("[LEDPANEL]::Initializing.");
    ledMatrix.begin();
    ledMatrix.setTextWrap(false);
    ledMatrix.setBrightness(40);
    ledMatrix.setTextColor(colors[0]);   
    ledMatrix.fillScreen(0); 
    ledMatrix.show();
    displayCenteredText("Boot", _color_white);
    ledMatrix.show();
    //ledMatrix.setFont(&Open_Sans_Condensed_Bold_8);
}



void ledpanel_task(void* parameters)
{
    while(true)
    {
        blinkCounter++;
        if(blinkCounter == 50)
        {
            ledPanelBlinkState = !ledPanelBlinkState;
            blinkCounter = 0;
        }

        ledMatrix.fillScreen(0);

        if(net_configApIsActive())
        {
            displayCenteredText("CFG AP", _color_red);

        }
        else if(!net_wifiIsConnected())
        {
            displayCenteredText("WiFi?", _color_yellow);
        }
        else if(!oscclient_server_is_known())
        {
            displayCenteredText("GW", ledPanelBlinkState ? _color_red : _color_white);
        }
        else
        {
            OnAirState state = onair_getstate();
            switch(state)
            {
                case OnAirState::OnAir:
                    displayCenteredText("LIVE", _color_red);
                    break;                
                case OnAirState::OffAir:
                    displayCenteredText("--:--", _color_blue);
                    break;
                case OnAirState::Countdown:
                    int count = (int)onair_getcount();
                    String countStr = (count > 9) ? String(count) : "0" + String(count);
                    displayCenteredText("00:" + countStr, _color_yellow);
                    break;
            }
        }

        // ledMatrix.setCursor(x, 0);
        // ledMatrix.print("Howdy");
        // if(--x < -36) {
        //     x = ledMatrix.width();
        //     if(++pass >= 3) pass = 0;
        //     ledMatrix.setTextColor(colors[pass]);
        // }
        ledMatrix.show();
        vTaskDelay(20);    
    }
}