#include <Arduino.h>

#ifndef __NET_H
#define __NET_H

void net_setup();
void net_task(void* parameters);

uint8_t net_getWifiRSSI();
bool net_configApIsActive();
bool net_wifiIsConnected();

#endif
