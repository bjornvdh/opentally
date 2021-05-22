#include <Arduino.h>

#ifndef __ATEMSWITCHER_H
#define __ATEMSWITCHER_H

void atemswitcher_setup();
void atemswitcher_task(void*);
bool atemswitcher_isenabled();
bool atemswithcer_isconnected();
#endif