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

void ModbusRTUSlave::poll() {
  ModbusADU adu;
  uint8_t error = _rtuComm.readAdu(adu);
  if (error) return;
  if (adu.getUnitId() != _unitId && adu.getUnitId() != 0) return;
  processPdu(adu);
  if (adu.getUnitId() != 0) {
    delay(_responseDelay);
    _rtuComm.writeAdu(adu);
  }
}