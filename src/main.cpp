#include <Arduino.h>
#include "hrsen.h"
#include "pms5505.h"
#include "LoRa.h"

HardwareSerial LoraSerial(1);
HardwareSerial PmsSerial(2);

void setup() {
  LoRasetup();
  LoraSerial.begin(9600,SERIAL_8N1, LORA_RX, LORA_TX);
  PmsSerial.begin(9600, SERIAL_8N1, PMS_RX, PMS_TX);
}

void loop() {
  
}

