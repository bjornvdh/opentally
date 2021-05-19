#include <Arduino.h>
#include <OSCMessage.h>

#ifndef __MESSAGEBUFFER_H
#define __MESSAGEBUFFER_H

struct MessageStruct
{
    String Address;
    IPAddress Recipient;
    int P1;
    String P2;
};

void messagebuffer_queuemessage(MessageStruct msg);
bool messagebuffer_ismessagewaiting();
MessageStruct messagebuffer_getnextmessage();

#endif