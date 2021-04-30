#include <Arduino.h>

#ifndef __OSCSERVER_H
#define __OSCSERVER_H

struct OSCSubscriber {
    IPAddress RemoteIP;
    uint8_t TallyChannel;
    unsigned long ValidUntil;
    unsigned long NextUpdate;
};

extern QueueHandle_t oscMessageQueue;
void oscserver_packetloop_task(void* parameters);

#endif