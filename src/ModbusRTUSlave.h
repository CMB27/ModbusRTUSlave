#ifndef ModbusRTUSlave_h
#define ModbusRTUSlave_h

#include "Arduino.h"

class ModbusRTUSlave {
  public:
    
    /**
     * This is the constructor. It sets up the modbus interface.
     * 
     * @param serial This specifies the Stream object to use.
     * @param buf This is a user defined byte array that is used as a buffer.
     * @param bufSize This is the size of the buffer array in bytes.
     * @param dePin This is optional. This specifies what pin to use to enable the driver on a RS-485 transceiver. No pin is used if this paramter is not set.
     */
    ModbusRTUSlave(Stream& serial, uint8_t *buf, uint16_t bufSize, uint8_t dePin = 255);
    
    /**
     * This is a definition for a function type. This type is used for reading coils and discrete inputs.
     * 
     * @return This returns a signed 8-bit value. It is normally used as a boolean value, but a neagaive value is used to indicate an error.
     * 
     * @code{.cpp}
     * char coilRead(word address) {
     * // code for reading a coil
     * }
     * @endcode
     */
    typedef int8_t (*BoolRead)(uint16_t);
    typedef bool (*BoolWrite)(uint16_t, bool);
    typedef int32_t (*WordRead)(uint16_t);
    typedef bool (*WordWrite)(uint16_t, uint16_t);
    
    /**
     * This configures the coils. If coils are used, this should be run in setup() before running poll().
     * 
     * @param numCoils This is the number of coils.
     * @param coilRead This is a user defined function of type BoolRead for reading coils.
     * @param coilWrite This is a user defined function of type BoolWrite for writing coils.
     */
    void configureCoils(uint16_t numCoils, BoolRead coilRead, BoolWrite coilWrite);
    
    /**
     * This configures discrete inputs. If discrete inputs are used, this should be run in setup() before running poll().
     * 
     * @param numDiscreteInputs This is the number of discrete inputs.
     * @param discreteInputRead This is a user defined function of type BoolRead for reading discrete inputs.
     */
    void configureDiscreteInputs(uint16_t numDiscreteInputs, BoolRead discreteInputRead);
    
    /**
     * This configures holding registers. If holding registers are used, this should be run in setup() before running poll().
     * 
     * @param numHoldingRegisters This is the number of holding registers.
     * @param holdingRegisterRead This is a user defined function of type WordRead for reading holding registers.
     * @param holdingRegisterWrite This is a user defined function of type WordWrite for writing holding registers.
     */
    void configureHoldingRegisters(uint16_t numHoldingRegisters, WordRead holdingRegisterRead, WordWrite holdingRegisterWrite);
    
    /**
     * This configures input registers. If input registers are used, this should be run in setup() before running poll().
     * 
     * @param numInputRegisters This is the number of input registers.
     * @param inputRegisterRead This is a user defined function of type WordRead for reading input registers.
     */
    void configureInputRegisters(uint16_t numInputRegisters, WordRead inputRegisterRead);
    
    /**
     * This should be run in setup() before running poll().
     * 
     * @param id This is the slave id/address.
     * @param baud This is the baud rate used in the serial interface.
     * @param config This is optional.
     */
    void begin(uint8_t id, uint32_t baud, uint8_t config = 0x06);
    
    /**
     * This should be run frequently in loop(). This processes requests.
     */
    void poll();
    
  private:
    Stream *_serial;
    uint8_t *_buf;
    uint16_t _bufSize;
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
    uint16_t _bytesToWord(uint8_t high, uint8_t low);
};

#endif
