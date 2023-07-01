#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h


#define MODBUS_RTU_SLAVE_BUF_SIZE 255
#define NO_DE_PIN 255
#define NO_ID 0

#include "Arduino.h"
#ifdef __AVR__
#include <SoftwareSerial.h>
#endif

class ModbusRTUSlave {
  public:
    ModbusRTUSlave(HardwareSerial& serial, uint8_t dePin = NO_DE_PIN);
    #ifdef __AVR__
    ModbusRTUSlave(SoftwareSerial& serial, uint8_t dePin = NO_DE_PIN);
    #endif
    #ifdef HAVE_CDCSERIAL
    ModbusRTUSlave(Serial_& serial, uint8_t dePin = NO_DE_PIN);
    #endif
    typedef int8_t (*BoolRead)(uint16_t);
    typedef bool (*BoolWrite)(uint16_t, bool);
    typedef int32_t (*WordRead)(uint16_t);
    typedef bool (*WordWrite)(uint16_t, uint16_t);
    void configureCoils(uint16_t numCoils, BoolRead coilRead, BoolWrite coilWrite);
    void configureDiscreteInputs(uint16_t numDiscreteInputs, BoolRead discreteInputRead);
    void configureHoldingRegisters(uint16_t numHoldingRegisters, WordRead holdingRegisterRead, WordWrite holdingRegisterWrite);
    void configureInputRegisters(uint16_t numInputRegisters, WordRead inputRegisterRead);
    void begin(uint8_t id, uint32_t baud, uint8_t config = SERIAL_8N1);
    void poll();
    
  private:
    HardwareSerial *_hardwareSerial;
    #ifdef __AVR__
    SoftwareSerial *_softwareSerial;
    #endif
    #ifdef HAVE_CDCSERIAL
    Serial_ *_usbSerial;
    #endif
    Stream *_serial;
    uint8_t _buf[MODBUS_RTU_SLAVE_BUF_SIZE];
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
    void _calculateTimeouts(uint32_t baud, uint8_t config);
    void _clearRxBuffer();
    bool _readRequest();
    void _writeResponse(uint8_t len);
    uint16_t _crc(uint8_t len);
    uint16_t _div8RndUp(uint16_t value);
    uint16_t _bytesToWord(uint8_t high, uint8_t low);
};

#endif
