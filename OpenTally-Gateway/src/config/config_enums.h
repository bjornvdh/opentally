#ifndef __CONFIG_ENUMS_H
#define __CONFIG_ENUMS_H

enum class ToggleModeEnum
{
    SetOff = 0,
    SetOn = 1,
    Toggle = 2
};

enum class LedMode
{
    None = 0,
    SelectedChannel = 1,            // LED Indicates the selected channel.
    ProgramPreview = 2,             // LED Indicates preview and program modes.
};

enum class KeyAction
{
    NoAction = 0,

    SelectChannel = 1,              // Select a tally-channel. 
                                    //      P1: Channel number.
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)

    SendSound = 2,                  // Send a command to the tally light, to make it emit a sound.   
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: The type of the sound.

    SendMessage = 3,                // Send a command to the tally light, to make it display a message.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: The message.

    SetPreview = 4,                 // Toggle the preview state for a tally channel.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)

    SetProgram = 5,                 // Toggle the program state for a tally channel.
                                    //      P1: Channel number (0 for currently selected channel).
                                    //      P2: Mode - 0 (off), 1 (on), 2 (toggle on/off)
    
    OnAirCountdown = 6,             // Toggle between "off air", "on air" and "count down to on air"
                                    //      P1: Mode - 0 (off air), 1 (count down), 2 (on air), 3 (cycle between off air, count down, on air)
};

#endif