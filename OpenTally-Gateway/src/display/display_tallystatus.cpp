#include <Arduino.h>
#include <TFT_eSPI.h>
#include "state/state_programpreview.h"
#include "state/state_client_channel.h"
#include "display/display_fonts.h"
#include "display_tallystatus.h"

extern TFT_eSPI tft;
extern bool displayBlinkState;

bool _last_preview_state = false;
bool _last_program_state = false;
int _last_channel_num = 255;

void display_tallystatus()
{
    int numChannel = state_getclientchannel();
    bool preview = state_getchannelpreviewstate(numChannel);
    bool program = state_getchannelprogramstate(numChannel);

    uint32_t bgcolor = TFT_NAVY;
    if(program)
    bgcolor = TFT_RED;
    else if(preview)
    bgcolor = 0xA4C0;    

    if(_last_channel_num != numChannel)
    {
        tft.loadFont(AA_FONT_SMALL);        
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        String numCameraString = String(numChannel);
        tft.fillRect(70,0,20,20, TFT_BLACK);
        tft.setCursor(80,2);
        tft.println(numCameraString);
        tft.unloadFont();
    }

    if(_last_preview_state != preview || _last_program_state != program || _last_channel_num != numChannel)
    {

        tft.fillRect(0, 20, tft.width(), tft.height() - 20, bgcolor);
        _last_program_state = program;
        _last_preview_state = preview;
    }

    _last_channel_num = numChannel;
}