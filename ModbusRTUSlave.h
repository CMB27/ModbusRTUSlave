#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"

#ifndef _MB_RTU_BUF_SIZE
#define _MB_RTU_BUF_SIZE 256
#endif

class ModbusRTUSlave {
  public:
    typedef int8_t (*BoolRead)(uint16_t);
    typedef bool (*BoolWrite)(uint16_t, bool);
    typedef int32_t (*WordRead)(uint16_t);
    typedef bool (*WordWrite)(uint16_t, uint16_t);
    
    ModbusRTUSlave(Stream& serial, uint8_t dePin = 255);
    void configureCoils(uint16_t numCoils, BoolRead coilRead, BoolWrite coilWrite);
    void configureDiscreteInputs(uint16_t numDiscreteInputs, BoolRead discreteInputRead);
    void configureHoldingRegisters(uint16_t numHoldingRegisters, WordRead holdingRegisterRead, WordWrite holdingRegisterWrite);
    void configureInputRegisters(uint16_t numInputRegisters, WordRead inputRegisterRead);
    void begin(uint8_t id, uint32_t baud, uint8_t config = 0x06);
    void poll();
  private:
    uint8_t _buf[_MB_RTU_BUF_SIZE];
    Stream *_serial;
    uint8_t _dePin;
    
    uint16_t _numCoils = 0;
    uint16_t _numDiscreteInputs = 0;
    uint16_t _numHoldingRegisters = 0;
    uint16_t _numInputRegisters = 0;
    
    BoolRead _coilRead;
    BoolRead _discreteInputRead;
    WordRead _holdingRegisterRead;
    WordRead _inputRegisterRead;
    BoolWrite _coilWrite;
    WordWrite _holdingRegisterWrite;
    
    uint8_t _id;
    uint32_t _charTimeout;
    uint32_t _frameTimeout;

    void _processBoolRead(uint16_t numBools, BoolRead boolRead);
    void _processWordRead(uint16_t numWords, WordRead wordRead);
    
    void _exceptionResponse(uint8_t code);
    void _write(uint8_t len);
    uint16_t _crc(uint8_t len);
    uint16_t _div8RndUp(uint16_t value);
    uint16_t _bytesToInt(uint8_t high, uint8_t low);
};

#endif
