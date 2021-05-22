#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <WiFiManager.h>

#include <buildconfig.h>

#include "state/state_client_channel.h"
#include "keypad/keypad.h"
#include "keypadeventhandler/keypadeventhandler.h"
#include "keypadleds/keypadleds.h"
#include "net/net.h"
#include "display/display.h"
#include "config/config.h"
#include "state/state_onair.h"
#include "state/state_programpreview.h"
#include "oscserver/oscserver.h"
#include "oscclient/oscclient.h"
#include "oscsender/oscsender.h"
#include "channelmessage/channelmessage.h"
#include "channelmessage/channelsound.h"
#include "tallyleds/tallyleds.h"
#include "atemswitcher/atemswitcher.h"
#include "ledpanel/ledpanel.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  
  display_setup();
  
  #ifdef HAS_LEDPANEL
    ledpanel_setup();
  #endif

  config_gateway_setup();
  keypad_setup();
  keypadeventhandler_setup();
  #ifdef HAS_KEYPAD_LEDS
    keypadleds_setup();
  #endif

  #ifdef HAS_TALLY_LEDS
    tallyleds_setup();
  #endif

  net_setup();
  state_clientchannel_setup();

  #ifdef IS_ONAIRCONTROLLER
    atemswitcher_setup();
  #endif

  
  xTaskCreatePinnedToCore(display_task, "DISP", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(keypad_task, "KEYPAD", 4096, NULL, 1, NULL, 1);  
  xTaskCreatePinnedToCore(keypadeventhandler_task, "KEYPADEVENTS", 4096, NULL, 1, NULL, 1);  
  
  #ifdef HAS_KEYPAD_LEDS
    xTaskCreatePinnedToCore(keypadleds_task, "LEDS", 2048, NULL, 1, NULL, 1);  
  #endif

  #ifdef HAS_BUZZER
    channelsound_setup();
  #endif
  
  xTaskCreatePinnedToCore(net_task, "NET", 4096, NULL, 1, NULL, 1);

  #ifdef IS_ONAIR_CONTROLLER
    xTaskCreatePinnedToCore(onair_task, "ONAIR", 8192, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(onair_refresh_task, "ONAIR_REFRESH", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(tallystate_refresh_task, "TALLY_REFRESH", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(atemswitcher_task, "ATEMSWITCHER", 16384, NULL, 1, NULL, 1);
  #endif

  #ifdef IS_OSCSERVER
    xTaskCreatePinnedToCore(oscserver_packetloop_task, "OSCSERVER_PACKET", 4096, NULL, 1, NULL, 1);
  #endif

  #ifdef IS_OSCCLIENT
    xTaskCreatePinnedToCore(oscclient_task, "OSCCLIENT_PACKET", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(oscclient_server_resolver_task, "OSCCLIENT_RESOLVER", 4096, NULL, 1, NULL, 1);
  #endif

  xTaskCreatePinnedToCore(oscsender_task, "OSCSENDER", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(channelmessage_task, "CHANNELMESSAGE", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(channelsound_task, "CHANNELSOUND", 2048, NULL, 1, NULL, 1);

  #ifdef HAS_LEDPANEL
    xTaskCreatePinnedToCore(ledpanel_task, "LEDPANEL", 8192, NULL, 1, NULL, 1);
  #endif
}

void loop() {

}