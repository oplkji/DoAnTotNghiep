#ifndef _LORA_H
#define _LORA_H
#include <Arduino.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#define M0 18
#define M1 19
#define LORA_TX 13
#define LORA_RX 14

void LoRaSetup(){
    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);
    digitalWrite(M0, LOW);
    digitalWrite(M1, LOW);
}

void sendMessage(HardwareSerial &LoRaSerial, DynamicJsonDocument &doc){
    String jsonParser;
    serializeJson(doc, jsonParser);
    LoRaSerial.println(jsonParser);
}
#endif