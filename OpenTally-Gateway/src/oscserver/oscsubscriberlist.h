#include <Arduino.h>
#include "config/config_enums.h"
#include "messagebuffer/messagebuffer.h"

#ifndef __OSCSUBSCRIBERLIST_H
#define __OSCSUBSCRIBERLIST_H

struct OSCSubscriber {
    IPAddress RemoteIP;
    uint8_t TallyChannel;
    unsigned long ValidUntil;
    OSCDeviceType DeviceType;
};

void subscriber_addOrRefresh(IPAddress remoteIp, OSCDeviceType deviceType, uint8_t tallyChannel);
void subscriber_queuemessageforall(MessageStruct msgTemplate);
void subscriber_queuemessageforchannel(MessageStruct msgTemplate, int channel);
void subscriber_queuemessagefordevicetype(MessageStruct msgTemplate, OSCDeviceType deviceType);

#endif