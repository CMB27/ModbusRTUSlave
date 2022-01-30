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
    ModbusRTUSlave(
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
      );
    void begin(uint8_t id, uint32_t baud, uint8_t config = 0x06);
    void poll();
  private:
    Stream *_serial;
    uint8_t *_buf;
    uint8_t _bufLen;
    bool *_coils;
    uint8_t _numCoils;
    bool *_inputs;
    uint8_t _numInputs;
    uint16_t *_hRegs;
    uint8_t _numHRegs;
    uint16_t *_inRegs;
    uint8_t _numInRegs;
    int16_t _dePin;
    uint8_t _id;
    uint32_t _charTimeout;
    uint32_t _frameTimeout;
    
    void _exceptionResponse(uint8_t code);
    void _write(uint8_t len);
    uint16_t _crc(uint8_t len);
    uint16_t _div8RndUp(uint16_t value);
    uint16_t _bytesToInt(uint8_t high, uint8_t low);
};

#endif
