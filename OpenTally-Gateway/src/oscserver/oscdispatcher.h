#include <Arduino.h>
#include "state/state_onair_enums.h"

#ifndef __OSCDISPATCHER_H
#define __OSCDISPATCHER_H

enum class OSCDispatchMessageType
{
    Subscribe = 0,
    OnAirState = 1,
    TallyState = 2,
    TextMessage = 3
};

extern QueueHandle_t oscOutgoingMessageQueue;
void oscdispatch_onairstate(OnAirState onAirState, int countDown);
void oscdispatch_tallystate(int tallyChannel, bool preview, bool program);

#endif
