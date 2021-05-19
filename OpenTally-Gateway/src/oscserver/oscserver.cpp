#include <Arduino.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include "buildconfig.h"
#include "oscserver/oscsubscriberlist.h"
#include "oscserver/oscserver.h"
#include "net/net.h"

WiFiUDP _server_udp;
OSCErrorCode _server_error;
bool _server_init_done = false;

void handleSubscribe(OSCMessage &msg)
{
    IPAddress remoteIP = _server_udp.remoteIP();

    Serial.print("OpenTally::Received subscribe from: ");
    Serial.print(_server_udp.remoteIP().toString());
    Serial.print(" devtype: ");
    Serial.println(msg.getInt(0));
    Serial.print(" for channel: ");
    char str[2];
    msg.getString(1, str, 2);
    Serial.println(str);
    
    subscriber_addOrRefresh(_server_udp.remoteIP(), (OSCDeviceType)msg.getInt(0),  String(str).toInt());
}

void initOscServer()
{
    _server_udp.begin(OPENTALLY_UDP_PORT);
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
                    Serial.println("[OSCServer]::Message received.");
                    msg.dispatch("/subscribe/device", handleSubscribe);
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