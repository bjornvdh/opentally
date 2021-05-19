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