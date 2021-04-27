#ifndef __CONFIG_H
#define __CONFIG_H

struct ConfigAction
{
    uint8_t Action;
    uint8_t ActionParam1;
    uint8_t ActionParam2;
};

void config_setup();
ConfigAction config_getkeypressedaction(uint8_t numKey);
extern bool configIsValid;

#define ACTION_SELECT_CHANNEL 1

#endif