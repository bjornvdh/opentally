#include <Arduino.h>
#ifndef __CONFIG_H
#define __CONFIG_H

struct ConfigAction
{
    int Action;
    int ActionParam1;
    String ActionParam2;
};

struct KeyConfig
{
    ConfigAction OnPress;
    ConfigAction OnLongpress;
};

void config_setup();
KeyConfig config_getkeyconfig(uint8_t numKey);
extern bool configIsValid;

#define ACTION_SELECT_CHANNEL 1     // Select a tally-channel. 
                                    //      P1: Channel number.
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)

#define ACTION_SEND_SOUND 2         // Send a command to the tally light, to make it emit a sound.   
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: The type of the sound.

#define ACTION_SEND_MESSAGE 3       // Send a command to the tally light, to make it display a message.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: The message.

#define ACTION_SET_PREVIEW 4        // Toggle the preview state for a tally channel.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)

#define ACTION_SET_PROGRAM 5        // Toggle the program state for a tally channel.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)

#endif