#include <Arduino.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#include "buildconfig.h"
#include "messagebuffer/messagebuffer.h"

WiFiUDP Udp;

void oscsender_task(void* params)
{
    while(true)
    {
        if(messagebuffer_ismessagewaiting())
        {
            MessageStruct msgDef = messagebuffer_getnextmessage();
            OSCMessage msg(msgDef.Address.c_str());
            msg.add(msgDef.P1);
            msg.add(msgDef.P2.c_str());
            msg.add(msgDef.P3.c_str());

            Udp.beginPacket(msgDef.Recipient, OPENTALLY_UDP_PORT);
            msg.send(Udp); 
            Udp.endPacket(); 
            msg.empty(); 

            // Serial.print("[OSCSENDER]::Message with address ");
            // Serial.print(msgDef.Address);
            // Serial.print(" sent to ");
            // Serial.print(msgDef.Recipient.toString());
            // Serial.println(" .");

            // Go do something else after the packet is put in the buffer
            taskYIELD();           
        }

        vTaskDelay(1);
    }
}