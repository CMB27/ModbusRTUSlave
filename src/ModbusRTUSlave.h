#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"
#include "ModbusADU.h"
#include "ModbusSlaveLogic.h"
#include "ModbusRTUComm.h"

enum ModbusRTUSlaveError : uint8_t {
  MODBUS_RTU_SLAVE_SUCCESS = 0,
  MODBUS_RTU_SLAVE_NO_REQUEST_AVAILABLE = 1,
  MODBUS_RTU_SLAVE_FRAME_ERROR = 2,
  MODBUS_RTU_SLAVE_CRC_ERROR = 3,
  MODBUS_RTU_SLAVE_UNKNOWN_COMM_ERROR = 4,
  MODBUS_RTU_SLAVE_ID_DOES_NOT_MATCH = 5,
  MODBUS_RTU_SLAVE_EXCEPTION_RESPONSE = 6
};

class ModbusRTUSlave : public ModbusSlaveLogic {
  public:
    ModbusRTUSlave(Stream& serial, int8_t dePin = -1, int8_t rePin = -1);
    void setResponseDelay(unsigned long responseDelay);
    void begin(uint8_t unitId, unsigned long baud, uint32_t config = SERIAL_8N1);
    ModbusRTUSlaveError poll();
    bool getBroadcastFlag();

  private:
    ModbusRTUComm _rtuComm;
    uint8_t _unitId = 0;
    unsigned long _responseDelay = 0;
    bool _broadcastFlag = false;

    ModbusRTUSlaveError _translateCommError(ModbusRTUCommError commError);

};

#endif
