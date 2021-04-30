#ifndef __DISPLAY_H
#define __DISPLAY_H

extern bool displayBlinkState;
extern bool sleepInitiated;

void display_request_refresh(bool clearScreen);
void display_setup();
void display_task(void* parameters);

#endif