#include "oscserver/oscdispatcher.h"
#include "oscserver/oscsubscriberlist.h"

void oscdispatch_onairstate(OnAirState onAirState, int countDown)
{
    //Serial.println("[OSCDispatcher]::Dispatch OnAir state message to all subscribers.");

    MessageStruct msg;
    msg.Address = "/state/onair";
    msg.P1 = (int)onAirState;
    msg.P2 = String(countDown);

    subscriber_queuemessageforall(msg);
}

void oscdispatch_tallystate(int tallyChannel, bool preview, bool program)
{
    MessageStruct msg;
    msg.Address = "/state/tally";
    msg.P1 = tallyChannel;
    msg.P2 = String((preview ? "1":"0")) + "|" + String((program ? "1": "0"));

    subscriber_queuemessageforchannel(msg, tallyChannel);
}

void oscdispatch_channelmessage(int tallyChannel, String message, uint32_t timeLeft)
{
    Serial.print("[OSCDISP]::Dispatching channel message for channel ");
    Serial.print(tallyChannel);
    Serial.print(" time ");
    Serial.print(String(timeLeft));
    Serial.println(".");
    MessageStruct msg;
    msg.Address = "/state/msg";
    msg.P1 = tallyChannel;
    msg.P2 = message;
    msg.P3 = String(timeLeft);

    subscriber_queuemessageforchannel(msg, tallyChannel);    
}

void oscdispatch_channelsound(int tallyChannel, uint32_t sound, uint32_t timeLeft)
{
    Serial.print("[OSCDISP]::Dispatching channel sound for channel ");
    Serial.print(tallyChannel);
    Serial.print(" time ");
    Serial.print(String(timeLeft));
    Serial.println(".");
    MessageStruct msg;
    msg.Address = "/state/sound";
    msg.P1 = tallyChannel;
    msg.P2 = String(sound);
    msg.P3 = String(timeLeft);

    subscriber_queuemessageforchannel(msg, tallyChannel);    
}