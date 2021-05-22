#include <Arduino.h>
#include <OSCMessage.h>

#ifndef __CHANNELSOUND_H
#define __CHANNELSOUND_H

void channelsound_receive(OSCMessage &msg);
void channelsound_set(int numChannel, int sound);
void channelsound_task(void* parameters);
void channelsound_setup();

#endif