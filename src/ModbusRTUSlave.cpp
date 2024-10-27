#include "ModbusRTUSlave.h"

ModbusRTUSlave::ModbusRTUSlave(Stream& serial, int8_t dePin, int8_t rePin) : _rtuComm(serial, dePin, rePin) {
  
}

void ModbusRTUSlave::configureCoils(bool coils[], uint16_t numCoils) {
  _coils = coils;
  _numCoils = numCoils;
}

void ModbusRTUSlave::configureDiscreteInputs(bool discreteInputs[], uint16_t numDiscreteInputs) {
  _discreteInputs = discreteInputs;
  _numDiscreteInputs = numDiscreteInputs;
}

void ModbusRTUSlave::configureHoldingRegisters(uint16_t holdingRegisters[], uint16_t numHoldingRegisters) {
  _holdingRegisters = holdingRegisters;
  _numHoldingRegisters = numHoldingRegisters;
}

void ModbusRTUSlave::configureInputRegisters(uint16_t inputRegisters[], uint16_t numInputRegisters) {
  _inputRegisters = inputRegisters;
  _numInputRegisters = numInputRegisters;
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
  switch (adu.getFunctionCode()) {
    case 1:
      _processReadCoils(adu);
      break;
    case 2:
      _processReadDiscreteInputs(adu);
      break;
    case 3:
      _processReadHoldingRegisters(adu);
      break;
    case 4:
      _processReadInputRegisters(adu);
      break;
    case 5:
      _processWriteSingleCoil(adu);
      break;
    case 6:
      _processWriteSingleHoldingRegister(adu);
      break;
    case 15:
      _processWriteMultipleCoils(adu);
      break;
    case 16:
      _processWriteMultipleHoldingRegisters(adu);
      break;
    default:
      adu.prepareExceptionResponse(1);
      break;
  }
  if (adu.getUnitId() != 0) {
    delay(_responseDelay);
    _rtuComm.writeAdu(adu);
  }
}

void ModbusRTUSlave::_processReadCoils(ModbusADU& adu) {
  _processReadValues(adu, _coils, _numCoils);
}

void ModbusRTUSlave::_processReadDiscreteInputs(ModbusADU& adu) {
  _processReadValues(adu, _discreteInputs, _numDiscreteInputs);
}

void ModbusRTUSlave::_processReadHoldingRegisters(ModbusADU& adu) {
  _processReadValues(adu, _holdingRegisters, _numHoldingRegisters);
}

void ModbusRTUSlave::_processReadInputRegisters(ModbusADU& adu) {
  _processReadValues(adu, _inputRegisters, _numInputRegisters);
}

void ModbusRTUSlave::_processWriteSingleCoil(ModbusADU& adu) {
  uint16_t address = adu.getDataRegister(0);
  uint16_t value = adu.getDataRegister(2);
  if (!_coils ||_numCoils == 0) adu.prepareExceptionResponse(1);
  else if (value != 0 && value != 0xFF00) adu.prepareExceptionResponse(3);
  else if (address >= _numCoils) adu.prepareExceptionResponse(2);
  else _coils[address] = value;
}

void ModbusRTUSlave::_processWriteSingleHoldingRegister(ModbusADU& adu) {
  uint16_t address = adu.getDataRegister(0);
  uint16_t value = adu.getDataRegister(2);
  if (!_holdingRegisters || _numHoldingRegisters == 0) adu.prepareExceptionResponse(1);
  else if (address >= _numHoldingRegisters) adu.prepareExceptionResponse(2);
  else _holdingRegisters[address] = value;
}

void ModbusRTUSlave::_processWriteMultipleCoils(ModbusADU& adu) {
  uint16_t startAddress = adu.getDataRegister(0);
  uint16_t quantity = adu.getDataRegister(2);
  if (!_coils || _numCoils == 0) adu.prepareExceptionResponse(1);
  else if (quantity == 0 || quantity > 1968 || adu.data[4] != div8RndUp(quantity)) adu.prepareExceptionResponse(3);
  else if (quantity > _numCoils || startAddress > (_numCoils - quantity)) adu.prepareExceptionResponse(2);
  else {
    for (uint16_t i = 0; i < quantity; i++) {
      _coils[startAddress + i] = bitRead(adu.data[5 + (i / 8)], i % 8);
    }
    adu.setDataLen(4);
  }
}

void ModbusRTUSlave::_processWriteMultipleHoldingRegisters(ModbusADU& adu) {
  uint16_t startAddress = adu.getDataRegister(0);
  uint16_t quantity = adu.getDataRegister(2);
  if (!_holdingRegisters || _numHoldingRegisters == 0) adu.prepareExceptionResponse(1);
  else if (quantity == 0 || quantity > 123 || adu.data[4] != (quantity * 2)) adu.prepareExceptionResponse(3);
  else if (quantity > _numHoldingRegisters || startAddress > (_numHoldingRegisters - quantity)) adu.prepareExceptionResponse(2);
  else {
    for (uint16_t i = 0; i < quantity; i++) {
      _holdingRegisters[startAddress + i] = adu.getDataRegister(5 + (i * 2));
    }
    adu.setDataLen(4);
  }
}



void ModbusRTUSlave::_processReadValues(ModbusADU& adu, bool buf[], uint16_t bufSize) {
  if (adu.getUnitId() == 0) return;
  uint16_t startAddress = adu.getDataRegister(0);
  uint16_t quantity = adu.getDataRegister(2);
  if (!buf || bufSize == 0) adu.prepareExceptionResponse(1);
  else if (quantity == 0 || quantity > 2000) adu.prepareExceptionResponse(3);
  else if (quantity > bufSize || startAddress > (bufSize - quantity)) adu.prepareExceptionResponse(2);
  else {
    uint8_t byteCount = div8RndUp(quantity);
    adu.data[0] = byteCount;
    for (uint16_t i = 0; i < (byteCount * 8); i++) {
      uint8_t byteIndex = 1 + (i / 8);
      uint8_t bitIndex = i % 8;
      if (i < quantity) bitWrite(adu.data[byteIndex], bitIndex, buf[startAddress + i]);
      else bitClear(adu.data[byteIndex], bitIndex);
    }
    adu.setDataLen(1 + byteCount);
  }
}

void ModbusRTUSlave::_processReadValues(ModbusADU& adu, uint16_t buf[], uint16_t bufSize) {
  if (adu.getUnitId() == 0) return;
  uint16_t startAddress = adu.getDataRegister(0);
  uint16_t quantity = adu.getDataRegister(2);
  if (!buf || bufSize == 0) adu.prepareExceptionResponse(1);
  else if (quantity == 0 || quantity > 125) adu.prepareExceptionResponse(3);
  else if (quantity > bufSize || startAddress > (bufSize - quantity)) adu.prepareExceptionResponse(2);
  else {
    uint8_t byteCount = quantity * 2;
    adu.data[0] = byteCount;
    for (uint16_t i = 0; i < quantity; i++) {
      adu.setDataRegister(1 + (i * 2), buf[startAddress + i]);
    }
    adu.setDataLen(1 + byteCount);
  }
}