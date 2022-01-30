#include "ModbusRTUSlave.h"

ModbusRTUSlave::ModbusRTUSlaveModbusRTUSlave(
      Stream& serial,
      uint8_t buf[],
      uint8_t bufLen,
      bool coils[],
      uint8_t numCoils,
      bool inputs[],
      uint8_t numInputs,
      bool hRegs[],
      uint8_t numHRegs,
      bool inRegs[],
      uint8_t numInRegs,
      uint8_t dePin = 255
      ) {
  _serial = &serial;
  _buf = buf;
  _bufLen = bufLen
  _coils = coils;
  _numCoils = numCoils;
  _inputs = inputs;
  _numInputs = numInputs;
  _hRegs = hRegs;
  _numHRegs = numHRegs;
  _numInRegs = numInRegs;
  _dePin = dePin;
}

void ModbusRTUSlave::begin(Stream& serial, uint32_t baud, uint8_t config) {
  _serial = &serial;
  uint32_t startTime = micros();
  if (baud > 19200) {
    _charTimeout = 750;
    _frameTimeout = 1750;
  }
  else if (config == SERIAL_8E2 or config == SERIAL_8O2) {
    _charTimeout = 18000000/baud;
    _frameTimeout = 42000000/baud;
  }
  else if (config == SERIAL_8N2 or config == SERIAL_8E1 or config == SERIAL_8O1) {
    _charTimeout = 16500000/baud;
    _frameTimeout = 38500000/baud;
  }
  else {
    _charTimeout = 15000000/baud;
    _frameTimeout = 35000000/baud;
  }
  if (_dePin >= 0) {
    digitalWrite(_dePin, LOW);
    pinMode(_dePin, OUTPUT);
  }
  do {
    if (_serial->available() > 0) {
      startTime = micros();
      _serial->read();
    }
  } while (micros() - startTime < _frameTimeout);
}

void ModbusRTUSlave::poll() {
  if (_serial->available() > 0) {
    
    // receive message
    uint8_t i = 0;
    uint32_t startTime;
    do {
      if (_serial->available() > 0) {
        startTime = micros();
        _buf[i] = _serial->read();
        i++;
      }
    } while (micros() - startTime < _charTimeout and i < 256);
    while (micros() - startTime < _frameTimeout);

    // check message
    if (_serial->available() > 0) return 0;
    if (_buf[0] != _id) return 0;
    if (_crc(i - 2) !=  _bytesToInt(_buf[i - 1], _buf[i - 2])) return 0;
    
    // process functions
    switch (_buf[1]) {

      case 1: /* Read Coils */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 2000) {
          _exceptionResponse(3);
          return;
        }
        if ((startingAddress + quantity) > _numCoils) {
          _exceptionResponse(2);
          return;
        }
        _buf[2] = _div8RndUp(quantity);
        for (uint8_t j = 0; j < quantity; j++) {
          bitWrite(_buf[3 + (j >> 3)], j & 7, _coils[startingAddress + j]);
        }
        _write(3 + _buf[2]);
        return;
      }

      case 2: /* Read Discrete Inputs */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 2000) {
          _exceptionResponse(3);
          return;
        }
        if ((startingAddress + quantity) > _numInputs) {
          _exceptionResponse(2);
          return;
        }
        _buf[2] = _div8RndUp(quantity);
        for (uint8_t j = 0; j < quantity; j++) {
          bitWrite(_buf[3 + (j >> 3)], j & 7, _inputs[startingAddress + j]);
        }
        _write(3 + _buf[2]);
        return;
      }
      
      case 3: /* Read Holding Registers */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 125) {
          _exceptionResponse(3);
          return;
        }
        if ((startingAddress + quantity) > _numHRegs) {
          _exceptionResponse(2);
          return;
        }
        _buf[2] = quantity * 2;
        for (uint8_t j = 0; j < quantity; j++) {
          _buf[3 + (j * 2)] = highByte(_hRegs[startingAddress + j]);
          _buf[4 + (j * 2)] = lowByte(_hRegs[startingAddress + j]);
        }
        _write(3 + _buf[2]);
        return;
      }
      
      case 4: /* Read Input Registers */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 125) {
          _exceptionResponse(3);
          return;
        }
        if ((startingAddress + quantity) > _numInRegs) {
          _exceptionResponse(2);
          return;
        }
        _buf[2] = quantity * 2;
        for (uint8_t j = 0; j < quantity; j++) {
          _buf[3 + (j * 2)] = highByte(_inRegs[startingAddress + j]);
          _buf[4 + (j * 2)] = lowByte(_inRegs[startingAddress + j]);
        }
        _write(3 + _buf[2]);
        return;
      }

      case 5: /* Write Single Coil */ {
        uint16_t address = _bytesToInt(_buf[2], _buf[3]);
        uint16_t value = _bytesToInt(_buf[4], _buf[5]);
        if (value != 0 and value != 0xFF00) {
          _exceptionResponse(3);
          return;
        }
        if (address >= _numCoils) {
          _exceptionResponse(2);
          return;
        }
        _coils[address] = value;
        _write(6);
        return;
      }
      
      case 6: /* Write Single Holding Register */ {
        uint16_t address = _bytesToInt(_buf[2], _buf[3]);
        uint16_t value = _bytesToInt(_buf[4], _buf[5]);
        if (address >= _numHRegs) {
          _exceptionResponse(2);
          return;
        }
        _hRegs[address] = value;
        _write(6);
        return;
      }

      case 15: /* Write Multiple Coils */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 1968 or _buf[6] != _div8RndUp(quantity)) {
          _exceptionResponse(3);
          return;
        }
        if ((startingAddress + quantity) > _numCoils) {
          _exceptionResponse(2);
          return;
        }
        for (uint8_t j = 0; j < quantity; j++) {
          _coils[startingAddress + j] = bitRead(_buf[7 + (j >> 3)], j & 7);
        }
        _write(6);
        return;
      }
      
      case 16: /* Write Multiple Holding Registers */ {
        uint16_t startingAddress = _bytesToInt(_buf[2], _buf[3]);
        uint16_t quantity = _bytesToInt(_buf[4], _buf[5]);
        if (quantity == 0 or quantity > 123 or _buf[6] != (quantity * 2)) {
          _exceptionResponse(3);
          return;
        }
        if (startingAddress + quantity > _numHRegs) {
          _exceptionResponse(2);
          return;
        }
        for (uint8_t j = 0; j < quantity; j++) {
          _hRegs[startingAddress + j] = _bytesToInt(_buf[j * 2 + 7], _buf[j * 2 + 8]);
        }
        _write(6);
        return;
      }
      
      default:
        _exceptionResponse(1);
        return;
    }
  }
}

void ModbusRTUSlave::_exceptionResponse(uint8_t code) {
  _buf[1] |= 0x80;
  _buf[2] = code;
  _write(3);
}

void ModbusRTUSlave::_write(uint8_t len) {
  uint16_t crc = _crc(len);
  _buf[len] = lowByte(crc);
  _buf[len + 1] = highByte(crc);
  if (_dePin >= 0) digitalWrite(_dePin, HIGH);
  _serial->write(_buf, len + 2);
  if (_dePin >= 0) digitalWrite(_dePin, LOW);
}

uint16_t ModbusRTUSlave::_crc(uint8_t len) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= (uint16_t)_buf[i];
    for (uint8_t j = 0; j < 8; j++) {
      bool lsb = crc & 1;
      crc >>= 1;
      if (lsb == true) {
        crc ^= 0xA001;
      }
    }
  }
  return crc;
}

uint16_t ModbusRTUSlave::_div8RndUp(uint16_t value) {
  return (value + 7) >> 3;
}

uint16_t ModbusRTUSlave::_bytesToInt(uint8_t high, uint8_t low) {
  return (high << 8) | low;
}
