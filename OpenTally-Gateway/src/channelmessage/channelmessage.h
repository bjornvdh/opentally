#include <Arduino.h>
#include "OSCMessage.h"

#ifndef __CHANNELMESSAGE_H
#define __CHANNELMESSAGE_H

void channelmessage_set(int numChannel, String message, uint32_t duration);
void channelmessage_task(void*);
void channelmessage_receive(OSCMessage &msg);
String channelmessage_get(int numChannel);
String channelmessage_getall();

#endif