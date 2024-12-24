#include "ModbusRTUSlave.h"

ModbusRTUSlave::ModbusRTUSlave(Stream& serial, int8_t dePin, int8_t rePin) : _rtuComm(serial, dePin, rePin) {
  
}

void ModbusRTUSlave::setResponseDelay(unsigned long responseDelay) {
  _responseDelay = responseDelay;
}

void ModbusRTUSlave::begin(uint8_t unitId, unsigned long baud, uint32_t config) {
  if (unitId >= 1 && unitId <= 247) _unitId = unitId;
  _rtuComm.begin(baud, config);
}

ModbusRTUSlaveError ModbusRTUSlave::poll() {
  clearDebugValues();
  _broadcastFlag = false;
  ModbusADU adu;
  ModbusRTUCommError error = _rtuComm.readAdu(adu);
  if (error) return _translateCommError(error);
  if (adu.getUnitId() != _unitId && adu.getUnitId() != 0) return MODBUS_RTU_SLAVE_ID_DOES_NOT_MATCH;
  processPdu(adu);
  if (adu.getUnitId() == 0) _broadcastFlag = true;
  else {
    delay(_responseDelay);
    _rtuComm.writeAdu(adu);
  }
  if (getExceptionResponse()) return MODBUS_RTU_SLAVE_EXCEPTION_RESPONSE;
  return MODBUS_RTU_SLAVE_SUCCESS;
}

bool ModbusRTUSlave::getBroadcastFlag() {
  return _broadcastFlag;
}



ModbusRTUSlaveError ModbusRTUSlave::_translateCommError(ModbusRTUCommError commError) {
  switch (commError) {
    case MODBUS_RTU_COMM_SUCCESS:
      return MODBUS_RTU_SLAVE_SUCCESS;
    case MODBUS_RTU_COMM_TIMEOUT:
      return MODBUS_RTU_SLAVE_NO_REQUEST_AVAILABLE;
    case MODBUS_RTU_COMM_FRAME_ERROR:
      return MODBUS_RTU_SLAVE_FRAME_ERROR;
    case MODBUS_RTU_COMM_CRC_ERROR:
      return MODBUS_RTU_SLAVE_CRC_ERROR;
    default:
      return MODBUS_RTU_SLAVE_UNKNOWN_COMM_ERROR;
  }
}
