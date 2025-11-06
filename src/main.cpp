#include <Arduino.h>
#include "hrsen.h"
#include "LoRa.h"
#include "PMS.h"

HardwareSerial LoraSerial(1);
HardwareSerial PmsSerial(2);

PMS pms(PmsSerial);
PMS::DATA data;
DynamicJsonDocument doc(200);
void setup() {
  LoRaSetup();
  Serial.begin(115200);
  LoraSerial.begin(9600,SERIAL_8N1, LORA_RX, LORA_TX);
  PmsSerial.begin(9600, SERIAL_8N1, PMS_RX, PMS_TX);
}

int prev_time = 0;
void loop() {
  if(pms.readUntil(data, 3000)){
    doc["PM1.0"] = data.PG_SP_UG_1_0;
    doc["PM2.5"] = data.PG_SP_UG_2_5;
    doc["PM10"] = data.PG_SP_UG_10_0;
    doc["Density"] = distance();
    doc["CO"] = "";
  }

  if(millis() - prev_time >= 30000){
    sendMessage(LoraSerial, doc);
  }
}

