#include <Arduino.h>

#ifndef __DISPLAY_GATEWAYSTATE_H
#define __DISPLAY_GATEWAYSTATE_H

void display_allmessages();
void display_mymessages(bool colorChanged, uint32_t bgcolor);

#endif