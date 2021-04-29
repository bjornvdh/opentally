#ifndef __UISTATE_PROGRAMREVIEW_H
#define __UISTATE_PROGRAMREVIEW_H

bool uistate_getchannelprogramstate(int numChannel);
bool uistate_getchannelpreviewstate(int numChannel);
bool uistate_setchannelprogramstate(int numChannel, bool state);
bool uistate_setchannelpreviewstate(int numChannel, bool state);

#endif