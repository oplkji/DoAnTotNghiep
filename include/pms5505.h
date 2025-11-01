
/*Example code
#include "PMS5003.h"

void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600, SERIAL_8N1, PMS_RX, PMS_TX);
  pinMode(PMS_SET, OUTPUT);
  digitalWrite(PMS_SET, HIGH);
}

void loop() {
  test(pmsSerial);
}
*/



#ifndef PMS5003_H
#define PMS5003_H
#include <Arduino.h>
#include <HardwareSerial.h>

#define PMS_RX   16
#define PMS_TX   17
#define PMS_SET  21

const int FRAME_LEN = 32;
uint8_t frame[FRAME_LEN];
unsigned long lastPrint = 0;

uint16_t calcChecksum(const uint8_t *buf, int len) {
  uint16_t sum = 0;
  for (int i = 0; i < len; i++) sum += buf[i];
  return sum;
}

void printHexBuf(const uint8_t *buf, int len) {
  for (int i = 0; i < len; i++) {
    if (buf[i] < 0x10) Serial.print('0');
    Serial.print(buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void test(HardwareSerial &pmsSerial) {  
  while (pmsSerial.available() >= 2) {
    int first = pmsSerial.peek();
    if (first == 0x42) {
      pmsSerial.read();
      int second = pmsSerial.peek();
      if (second == 0x4D) {
        pmsSerial.read();
        if (pmsSerial.readBytes(frame + 2, FRAME_LEN - 2) == FRAME_LEN - 2) {
          frame[0] = 0x42;
          frame[1] = 0x4D;

          uint16_t sum = calcChecksum(frame, FRAME_LEN - 2);
          uint16_t chk = ((uint16_t)frame[30] << 8) | frame[31];

          if (sum == chk) {
            uint16_t pm1_cf1  = ((uint16_t)frame[4] << 8)  | frame[5];
            uint16_t pm25_cf1 = ((uint16_t)frame[6] << 8)  | frame[7];
            uint16_t pm10_cf1 = ((uint16_t)frame[8] << 8)  | frame[9];
            uint16_t pm1_atm  = ((uint16_t)frame[10] << 8) | frame[11];
            uint16_t pm25_atm = ((uint16_t)frame[12] << 8) | frame[13];
            uint16_t pm10_atm = ((uint16_t)frame[14] << 8) | frame[15];

            Serial.println(F("---- PMS5003 DATA ----"));
            Serial.printf("PM1.0 (CF=1): %u\n", pm1_cf1);
            Serial.printf("PM2.5 (CF=1): %u\n", pm25_cf1);
            Serial.printf("PM10.0 (CF=1): %u\n", pm10_cf1);
            Serial.printf("PM1.0 (atm): %u\n", pm1_atm);
            Serial.printf("PM2.5 (atm): %u\n", pm25_atm);
            Serial.printf("PM10.0 (atm): %u\n", pm10_atm);
            Serial.println(F("----------------------"));
            lastPrint = millis();
          } else {
            Serial.println(F("Checksum mismatch"));
            printHexBuf(frame, FRAME_LEN);
          }
        }
      } else {
        pmsSerial.read();
      }
    } else {
      pmsSerial.read();
    }
  }

  if (millis() - lastPrint > 3000) {
    Serial.print('.');
    lastPrint = millis();
  }
}

#endif