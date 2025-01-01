#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"
#include "ModbusADU.h"
#include "ModbusSlaveLogic.h"
#include "ModbusRTUComm.h"

class ModbusRTUSlave : public ModbusSlaveLogic {
  public:
    ModbusRTUSlave(Stream& serial, int8_t dePin = -1, int8_t rePin = -1);
    void setResponseDelay(unsigned long responseDelay);
    void begin(uint8_t localUnitId, unsigned long baud, uint32_t config = SERIAL_8N1);
    bool poll();

  private:
    ModbusRTUComm _rtuComm;
    uint8_t _localUnitId = 0;
    unsigned long _responseDelay = 0;
    using ModbusSlaveLogic::processPdu;

};

#endif
