#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <WiFiManager.h>

#include <buildconfig.h>

#include "keypad/keypad.h"
#include "keypadeventhandler/keypadeventhandler.h"
#include "keypadleds/keypadleds.h"
#include "net/net.h"
#include "display/display.h"
#include "config/config.h"
#include "state/state_onair.h"
#include "oscserver/oscserver.h"
#include "oscclient/oscclient.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  
  display_setup();
  config_gateway_setup();
  keypad_setup();
  keypadeventhandler_setup();
  keypadleds_setup();
  net_setup();

  xTaskCreatePinnedToCore(display_task, "DISP", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(keypad_task, "KEYPAD", 2048, NULL, 1, NULL, 1);  
  xTaskCreatePinnedToCore(keypadeventhandler_task, "KEYPADEVENTS", 2048, NULL, 1, NULL, 1);  
  
  #ifdef HAS_KEYPAD_LEDS
    xTaskCreatePinnedToCore(keypadleds_task, "LEDS", 2048, NULL, 1, NULL, 1);  
  #endif
  
  xTaskCreatePinnedToCore(net_task, "NET", 4096, NULL, 1, NULL, 1);

  #ifdef IS_ONAIR_CONTROLLER
    xTaskCreatePinnedToCore(onair_task, "ONAIR", 1024, NULL, 1, NULL, 1);
  #endif

  #ifdef IS_OSCSERVER
    xTaskCreatePinnedToCore(oscserver_packetloop_task, "OSCSERVER_PACKET", 4096, NULL, 1, NULL, 1);
  #endif

  #ifdef IS_OSCCLIENT
    xTaskCreatePinnedToCore(oscclient_task, "OSCCLIENT_PACKET", 4096, NULL, 1, NULL, 1);
  #endif
}

void loop() {

}