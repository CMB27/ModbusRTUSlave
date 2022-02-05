#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"

class ModbusRTUSlave {
  public:
    ModbusRTUSlave(
      Stream& serial,
      uint8_t *buf, size_t bufLen,
      bool *coils, size_t numCoils,
      bool *inputs, size_t numInputs,
      uint16_t *hRegs, size_t numHRegs,
      uint16_t *inRegs, size_t numInRegs,
      uint8_t dePin = 255
      );
    void begin(uint8_t id, uint32_t baud, uint8_t config = 0x06);
    void poll();
  private:
    Stream *_serial;
    uint8_t *_buf;
    size_t _bufLen;
    bool *_coils;
    size_t _numCoils;
    bool *_inputs;
    size_t _numInputs;
    uint16_t *_hRegs;
    size_t _numHRegs;
    uint16_t *_inRegs;
    size_t _numInRegs;
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
