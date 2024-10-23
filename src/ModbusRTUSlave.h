#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"
#include "ModbusADU.h"
#include "ModbusRTUComm.h"

class ModbusRTUSlave {
  public:
    ModbusRTUSlave(Stream& serial, int8_t dePin = -1, int8_t rePin = -1);
    void configureCoils(bool coils[], uint16_t numCoils);
    void configureDiscreteInputs(bool discreteInputs[], uint16_t numDiscreteInputs);
    void configureHoldingRegisters(uint16_t holdingRegisters[], uint16_t numHoldingRegisters);
    void configureInputRegisters(uint16_t inputRegisters[], uint16_t numInputRegisters);
    void setResponseDelay(unsigned long responseDelay);
    void begin(uint8_t unitId, unsigned long baud, uint32_t config = SERIAL_8N1);
    void poll();

  private:
    ModbusRTUComm _rtuComm;
    bool *_coils = 0;
    bool *_discreteInputs = 0;
    uint16_t *_holdingRegisters = 0;
    uint16_t *_inputRegisters = 0;
    uint16_t _numCoils = 0;
    uint16_t _numDiscreteInputs = 0;
    uint16_t _numHoldingRegisters = 0;
    uint16_t _numInputRegisters = 0;
    uint8_t _unitId = 0;
    unsigned long _responseDelay = 0;

    void _processReadCoils(ModbusADU& adu);
    void _processReadDiscreteInputs(ModbusADU& adu);
    void _processReadHoldingRegisters(ModbusADU& adu);
    void _processReadInputRegisters(ModbusADU& adu);
    void _processWriteSingleCoil(ModbusADU& adu);
    void _processWriteSingleHoldingRegister(ModbusADU& adu);
    void _processWriteMultipleCoils(ModbusADU& adu);
    void _processWriteMultipleHoldingRegisters(ModbusADU& adu);

    void _processReadValues(ModbusADU& adu, bool buf[], uint16_t bufSize);
    void _processReadValues(ModbusADU& adu, uint16_t buf[], uint16_t bufSize);

};

#endif
