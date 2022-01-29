#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define SERIAL_8N1 0x06
#define SERIAL_8N2 0x0E
#define SERIAL_8E1 0x26
#define SERIAL_8E2 0x2E
#define SERIAL_8O1 0x36
#define SERIAL_8O2 0x3E

class ModbusRTUSlave {
  public:
    ModbusRTUSlave(uint8_t id, uint8_t numCoils, uint8_t numDiscreteInputs, uint8_t numHoldingRegisters, uint8_t numInputRegisters, int16_t dePin = -1);
    bool coilRead(uint8_t address);
    bool discreteInputRead(uint8_t address);
    uint16_t holdingRegisterRead(uint8_t address);
    uint16_t inputRegisterRead(uint8_t address);
    void coilWrite(uint8_t address, bool value);
    void discreteInputWrite(uint8_t address, bool value);
    void holdingRegisterWrite(uint8_t address, uint16_t value);
    void inputRegisterWrite(uint8_t address, uint16_t value);
    void begin(Stream& serial, uint32_t baud, uint8_t config = SERIAL_8N1);
    void poll();
  private:
    Stream *_serial;
    uint8_t _id;
    uint8_t _numCoils;
    uint8_t _numDiscreteInputs;
    uint8_t _numHoldingRegisters;
    uint8_t _numInputRegisters;
    int16_t _dePin;
    uint32_t _charTimeout;
    uint32_t _frameTimeout;
    
    uint8_t _buffer[256];
    bool *_coils;
    bool *_discreteInputs;
    uint16_t *_holdingRegisters;
    uint16_t *_inputRegisters;
    
    void _exceptionResponse(uint8_t code);
    void _write(uint8_t len);
    uint16_t _crc(uint8_t len);
    uint16_t _div8RndUp(uint16_t value);
    uint16_t _bytesToInt(uint8_t high, uint8_t low);
};

#endif
