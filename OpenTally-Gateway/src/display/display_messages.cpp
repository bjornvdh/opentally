#include <Arduino.h>
#include <TFT_eSPI.h>
#include "oscclient/oscclient.h"
#include "display/display_fonts.h"
#include "display_tallystatus.h"
#include "channelmessage/channelmessage.h"
#include "state/state_client_channel.h"

extern TFT_eSPI tft;

String _lastMessage;
TFT_eSprite textSprite = TFT_eSprite(&tft);
int _lastTextHeight = 25;
const int _MESSAGE_TEXT_TOP = 40;
const int _ALL_MESSAGES_TEXT_TOP = 30;

void display_allmessages()
{
    String messages = channelmessage_getall();
    if(messages != _lastMessage)
    {
        tft.setTextFont(2);
        tft.fillRect(0,_ALL_MESSAGES_TEXT_TOP - 10,tft.width(), tft.height(), TFT_BLACK);
        tft.setCursor(0, _ALL_MESSAGES_TEXT_TOP);
        tft.setTextColor(TFT_WHITE);
        tft.println(messages);
        _lastMessage = messages;
    }

}

void display_mymessages(bool forceRefresh, uint32_t bgcolor)
{
    int numChannel = state_getclientchannel();
    String message = channelmessage_get(numChannel);
    if(forceRefresh || _lastMessage != message )
    {
        tft.fillRect(0,_MESSAGE_TEXT_TOP - 10,tft.width(), _lastTextHeight + 20,bgcolor);

        if(!message.isEmpty())
        {
            Serial.print("Printing message: ");
            Serial.println(message);        
            uint32_t foreColor = TFT_BLACK;
            uint32_t backColor = TFT_YELLOW;

            textSprite.createSprite(tft.width() - 20, tft.height() - 60);
            //textSprite.loadFont(AA_FONT_MEDIUM);
            textSprite.setTextDatum(TL_DATUM);
            textSprite.fillSprite(TFT_CYAN);
            textSprite.setTextFont(4);
            //textSprite.setTextSize(2);
            textSprite.setTextColor(foreColor, backColor);
            textSprite.setTextWrap(true);
            textSprite.setCursor(0,0);
            textSprite.println(message);
            int textHeight = (textSprite.getCursorY());
            _lastTextHeight = textHeight;
            textSprite.unloadFont();

            tft.fillRoundRect(6,_MESSAGE_TEXT_TOP - 9,tft.width() - 12, (textHeight + 18),5,backColor);
            tft.drawRoundRect(5,_MESSAGE_TEXT_TOP - 10,tft.width() - 10, (textHeight + 20),5,TFT_BLACK);
            textSprite.pushSprite(10,_MESSAGE_TEXT_TOP,TFT_CYAN);
        }

        _lastMessage = message;
    }
}