#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <WiFiManager.h>

#include "keypad/keypad.h"

void setup() {
  keypad_setup();
}

void loop() {
    xTaskCreatePinnedToCore(keypad_task, "KEYPAD", 2048, NULL, 1, NULL, 1);  
}