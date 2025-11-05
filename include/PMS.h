#ifndef PMS_H
#define PMS_H

/*
Example Code:


#include <HardwareSerial.h>
#include "PMS.h"

#define PMS_RX   18   // connect to PMS TX
#define PMS_TX   17   // connect to PMS RX (optional)
#define PMS_SET  21   // connect to PMS SET pin

HardwareSerial pmsSerial(2);      // using UART2
PMS pms(pmsSerial);
PMS::DATA data;

void setup() {

  Serial.begin(115200);
  pmsSerial.begin(PMS::baudRate SERIAL_8N1, PMS_RX, PMS_TX);
  


  Serial.println("Listening for PMS5003 frames...");
}

void loop() {
  if (pms.readUntil(data, 2000)) {  // wait up to 2 seconds for a full frame
    Serial.println("Got data!");
    Serial.print("PM 1.0 (ug/m3): "); Serial.println(data.PG_SP_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): "); Serial.println(data.PG_SP_UG_2_5);
    Serial.print("PM 10  (ug/m3): "); Serial.println(data.PG_SP_UG_10_0);
  } else {
    Serial.println("No valid data yet...");
  }

  delay(2000);
}

    */

#include <Arduino.h>
#include <Stream.h>

class PMS {
public:
    static const uint16_t baudRate = 9600;
    
    PMS(Stream&);

    struct DATA{
        uint16_t PG_SP_UG_1_0;
        uint16_t PG_SP_UG_2_5;
        uint16_t PG_SP_UG_10_0;

        uint16_t PG_AE_UG_1_0;
        uint16_t PG_AE_UG_2_5;
        uint16_t PG_AE_UG_10_0;

    };

    // Sleep mode. For low power consumption and prolong the life of the sensor.
    // Mode này quạt không chạy.
    void sleep();
    // Operating mode. 
    // Stable data should be got at least 30 seconds after the sensor wakeup from the sleep mode because of the fan's performance.
    void wakeup();
    
    //Hai chế độ truyền dữ liệu:
    // - Active Mode: Sensor sẽ auto truyển liên tục đến serial. (mode default lúc bật lên)
    // - Passive Mode: Sensor chỉ truyền dữ liệu theo request.
    void activeMode();
    void passiveMode();
    String getMode() {return ModeToString(_mode);};
    void requestData();
    
    void parse();
    bool read(DATA&);
    bool readUntil(DATA&,u_int16_t);
    

private:
    Stream* _stream;    
    u_int8_t _index = 0;
    u_int16_t _frameLen;
    u_int16_t _checksum;
    u_int16_t _calculatedChecksum;
    uint8_t _payload[12];

    enum STATUS {
        STATUS_WAITING,
        STATUS_OK
    };
    STATUS _status;

    enum MODE{
        ACTIVE_MODE,
        PASSIVE_MODE
    };

    constexpr const char* ModeToString(MODE mode) noexcept {
        return (mode == MODE::ACTIVE_MODE)  ? "Active Mode" :
            (mode == MODE::PASSIVE_MODE) ? "Passive Mode" :
                                            "Unknown Mode";
    }


    MODE _mode = ACTIVE_MODE;
    DATA* _data;
};

#endif
