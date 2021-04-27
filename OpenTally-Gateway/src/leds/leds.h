#ifndef __LEDS_H
#define __LEDS_H

void leds_request_refresh();
void leds_setup();
void leds_task(void* parameters);

#endif