#include "PMS.h"


PMS::PMS(Stream& stream){
    this->_stream = &stream;
}


void PMS::sleep(){
    uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73 };
    this->_stream->write(command, sizeof(command));
}


void PMS::wakeup(){
    uint8_t command[] = {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x74};
    this->_stream->write(command,sizeof(command));
}

void PMS::activeMode(){
    this->_mode = ACTIVE_MODE;
    uint8_t command[] = {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71};
    this->_stream->write(command, sizeof(command));
}

void PMS::passiveMode(){
    this->_mode = PASSIVE_MODE;
    uint8_t command[] = {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70};
    this->_stream->write(command, sizeof(command));
}

void PMS::requestData(){
    if(this->_mode != PASSIVE_MODE){
        return;
    }
    uint8_t command[] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71};
    this->_stream->write(command, sizeof(command));
}


bool PMS::read(DATA& data)
{
  _data = &data;
  this->parse();
  
  return _status == STATUS_OK;
}

bool PMS::readUntil(DATA& data, u_int16_t timeout){
    this->_data = &data;
    uint32_t start = millis();
    do {
        parse();
    } while (millis() -start <timeout);
    return this->_status == STATUS_OK;
}

void PMS::parse(){
    if(this->_stream->available()){
        this->_status = STATUS_WAITING;
        uint8_t ch = this->_stream->read();
        switch(_index){
            case 0:{
                if(ch != 0x42) return;
                this->_calculatedChecksum = ch;
                break;
            }
            case 1:{
                if(ch != 0x4D){
                    this->_index = 0;
                    return;
                }
                this->_calculatedChecksum +=ch;
                break;
            }
            case 2:{
                this->_calculatedChecksum +=ch;
                this->_frameLen = ch << 8;
                break;
            }
            case 3:{
                this->_frameLen |= ch;
                this->_calculatedChecksum +=ch;
                if (_frameLen != 2 * 9 + 2 && _frameLen != 2 * 13 + 2) {
                    _index = 0;
                    return;
                }
                break;
            }
            default:{
                if(this->_index == this->_frameLen +2){
                    _checksum = ch << 8;
                } else if (this->_index == this->_frameLen +2 +1){
                    _checksum |= ch;

                    if(this->_calculatedChecksum == _checksum){
                        this->_status = STATUS_OK;

                        this->_data->PG_SP_UG_1_0 = makeWord(_payload[0],_payload[1]);
                        this->_data->PG_SP_UG_2_5 = makeWord(_payload[2],_payload[3]);
                        this->_data->PG_SP_UG_10_0 = makeWord(_payload[4],_payload[5]);

                        this->_data->PG_AE_UG_1_0 = makeWord(_payload[6],_payload[7]);
                        this->_data->PG_AE_UG_2_5 = makeWord(_payload[8],_payload[9]);
                        this->_data->PG_AE_UG_10_0 = makeWord(_payload[10],_payload[11]);
                        this->_index=0;
                        return;
                    } else {
                        this->_index=0;
                    }
                } else {
                    this->_calculatedChecksum += ch;
                    u_int16_t payloadIndex = this->_index -4; // Trừ phần header
                    
                    if(payloadIndex < sizeof(this->_payload)){
                        this->_payload[payloadIndex] = ch;
                    }
                }
                break;
            }
        }
        this->_index++;
    }
}






