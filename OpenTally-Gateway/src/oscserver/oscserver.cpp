#include <Arduino.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include "oscserver/oscsubscriberlist.h"
#include "oscserver/oscserver.h"
#include "net/net.h"





WiFiUDP _server_udp;
OSCErrorCode _server_error;
bool _server_init_done = false;

void handleSubscribe(OSCMessage &msg)
{
    unsigned long time = millis();
    IPAddress remoteIP = _server_udp.remoteIP();

    Serial.print("OpenTally::Received subscribe from: ");
    Serial.print(_server_udp.remoteIP().toString());
    Serial.print(" for channel: ");
    Serial.println(msg.getInt(0));

    

    
}

void initOscServer()
{
    _server_udp.begin(13323);
    Serial.println("OpenTally::Server listening.");
    _server_init_done = true;    
}


void oscserver_packetloop_task(void* parameters)
{
    while(true)
    {
        if(net_wifiIsConnected())
        {
            if(!_server_init_done) initOscServer();

            OSCMessage msg;
            int size = _server_udp.parsePacket();

            if (size > 0) {
                while (size--) {
                msg.fill(_server_udp.read());
                }
                if (!msg.hasError()) {
                    //Serial.println("OpenTally::Message received.");
                    msg.dispatch("/subscribe/tally", handleSubscribe);
                } else {
                    _server_error = msg.getError();
                    Serial.print("OpenTally::error: ");
                    Serial.println(_server_error);
                }
            }
        }
        else
        {
            _server_init_done = false;
        }
    }
}