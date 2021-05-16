#include <Arduino.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include "oscclient/oscclient.h"
#include "net/net.h"

#define SUBSCRIBE_INTERVAL  20000


WiFiUDP _client_udp;
OSCErrorCode _client_error;
bool _client_init_done = false;
uint32_t _next_subscribe = 0;

void initOscClient()
{
    _client_udp.begin(13323);
    Serial.println("OpenTally::Client listening.");
    _client_init_done = true;    
}


void sendSubscribe()
{

}

void oscclient_task(void* parameters)
{
    while(true)
    {
        if(net_wifiIsConnected())
        {
            if(!_client_init_done) initOscClient();

            if(_next_subscribe < millis()) sendSubscribe();

            OSCMessage msg;
            int size = _client_udp.parsePacket();

            if (size > 0) {
                while (size--) {
                msg.fill(_client_udp.read());
                }
                if (!msg.hasError()) {
                    //Serial.println("OpenTally::Message received.");
                    //msg.dispatch("/subscribe/tally", handleSubscribe);
                } else {
                    _client_error = msg.getError();
                    Serial.print("OpenTally::error: ");
                    Serial.println(_client_error);
                }
            }
        }
        else
        {
            _client_init_done = false;
        }
    }
}