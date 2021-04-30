#ifndef __state_PROGRAMREVIEW_H
#define __state_PROGRAMREVIEW_H

bool state_getchannelprogramstate(int numChannel);
bool state_getchannelpreviewstate(int numChannel);
void state_setchannelprogramstate(int numChannel, bool state);
void state_setchannelpreviewstate(int numChannel, bool state);

#endif