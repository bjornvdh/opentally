#include <Arduino.h>

#ifndef __OSCSUBSCRIBERLIST_H
#define __OSCSUBSCRIBERLIST_H

enum class OSCDeviceType {
    TallyLight = 0,
    OnAirDisplay = 1,
    Gateway = 2,
};

struct OSCSubscriber {
    IPAddress RemoteIP;
    uint8_t TallyChannel;
    unsigned long ValidUntil;
    OSCDeviceType DeviceType;
};

void subscriber_addOrRefresh(IPAddress remoteIp, OSCDeviceType deviceType, uint8_t tallyChannel);



#endif