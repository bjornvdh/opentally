#ifndef __KEYPAD_LEDS_H
#define __KEYPAD_LEDS_H

void keypadleds_request_refresh();
void keypadleds_setup();
void keypadleds_task(void* parameters);

#endif