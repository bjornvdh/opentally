#include <Arduino.h>
#include <OSCMessage.h>

#ifndef __state_PROGRAMREVIEW_H
#define __state_PROGRAMREVIEW_H

bool state_getchannelprogramstate(int numChannel);
bool state_getchannelpreviewstate(int numChannel);
void state_setchannelprogramstate(int numChannel, bool state);
void state_setchannelpreviewstate(int numChannel, bool state);
void state_setchannelprogrampreviewstate(int numChannel, bool preview, bool program);
void tallystate_refresh_task(void*);
void state_receivetallystate(OSCMessage &msg);

#endif