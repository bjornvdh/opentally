#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <mdns.h>

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include "buildconfig.h"
#include "oscclient/oscclient.h"
#include "net/net.h"
#include "state/state_client_channel.h"
#include "state/state_onair.h"
#include "state/state_programpreview.h"
#include "messagebuffer/messagebuffer.h"
#include "display/display.h"
#include "channelmessage/channelmessage.h"

#define SUBSCRIBE_INTERVAL  20000


WiFiUDP _client_udp;
OSCErrorCode _client_error;
bool _client_init_done = false;
uint32_t _next_subscribe = 0;
IPAddress _osc_server_ip;
bool _osc_server_is_known = false;
bool _request_resubscribe = false;

static SemaphoreHandle_t oscClientMutex = xSemaphoreCreateMutex();


void initOscClient()
{
    _client_udp.begin(OPENTALLY_UDP_PORT);
    Serial.println("OpenTally::Client listening.");
    _client_init_done = true;    
}

bool oscclient_server_is_known()
{
    bool retVal = false;
    xSemaphoreTake(oscClientMutex, portMAX_DELAY);
    retVal = _osc_server_is_known;
    xSemaphoreGive(oscClientMutex);
    return retVal;
}

IPAddress OscServerIp()
{
    IPAddress retVal;
    xSemaphoreTake(oscClientMutex, portMAX_DELAY);
    retVal = _osc_server_ip;
    xSemaphoreGive(oscClientMutex);
    return retVal;
}

void resolveOscServer()
{
    Serial.println("[OSCClient resolver]::Resolving OSC server...");
    ip4_addr_t oscServerAddr;
    esp_err_t retVal = mdns_query_a(GATEWAY_HOSTNAME, 10000, &oscServerAddr);
    bool knownValueChanged = false;

    if(retVal == ESP_OK)
    {
        Serial.println("[OSCClient resolver]::OSC Server found!");
        xSemaphoreTake(oscClientMutex, portMAX_DELAY);
        _osc_server_ip = IPAddress(oscServerAddr.addr);
        knownValueChanged = !_osc_server_is_known;
        _osc_server_is_known = true;
        xSemaphoreGive(oscClientMutex);

        Serial.print("[OSCClient resolver]::OSC ip: ");
        Serial.println(_osc_server_ip);
    }
    else
    {
        xSemaphoreTake(oscClientMutex, portMAX_DELAY);
        _osc_server_is_known = false;
        knownValueChanged = _osc_server_is_known;
        xSemaphoreGive(oscClientMutex);
        Serial.println("[OSCClient resolver]::OSC Server not found!");
    }

    if(knownValueChanged) display_request_refresh(false);
}


void sendSubscribe()
{   
    MessageStruct subMsg;
    subMsg.Address = "/subscribe/device";
    subMsg.P1 = OPENTALLY_DEVICE_TYPE;
    subMsg.P2 = String(state_getclientchannel());
    subMsg.Recipient = OscServerIp();
    
    messagebuffer_queuemessage(subMsg);
    Serial.println("[OSCClient]::Subscribe message queued.");
    _next_subscribe = millis() + SUBSCRIBE_INTERVAL;

    _request_resubscribe = false;
}

void oscclient_server_resolver_task(void* parameters)
{
    while(true)
    {
        if(net_wifiIsConnected())
            resolveOscServer();  
        
        if(!_osc_server_is_known)
            vTaskDelay(5000);
        else
            vTaskDelay(30000);
    }
}

void oscclient_request_resubscribe()
{
    _request_resubscribe = true;
}

void oscclient_task(void* parameters)
{
    while(true)
    {
        if(net_wifiIsConnected())
        {
            if(!_client_init_done) initOscClient();

            if(oscclient_server_is_known() && (_next_subscribe < millis() || _request_resubscribe)) sendSubscribe();

            OSCMessage msg;
            int size = _client_udp.parsePacket();

            if (size > 0) {
                while (size--) {
                    msg.fill(_client_udp.read());
                }
                if (!msg.hasError()) {
                    Serial.println("[OSCCLIENT]::Message received.");
                    //msg.dispatch("/subscribe/tally", handleSubscribe);
                    msg.dispatch("/state/onair", onair_receivestate);
                    msg.dispatch("/state/tally", state_receivetallystate);
                    msg.dispatch("/state/msg", channelmessage_receive);
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
        vTaskDelay(1);
    }
}