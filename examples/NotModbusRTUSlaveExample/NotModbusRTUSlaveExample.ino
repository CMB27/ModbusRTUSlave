/*
  NotModbusRTUSlaveExample

  This example should do the same thing as ModbusRTUSlaveExample, but without actually using the ModbusRTUSlave library.
  This adds some complexity, but it exposes more of the process and more information that can be used for debugging or advanced applications.

  See ModbusRTUSlaveExample README (https://github.com/CMB27/ModbusRTUMaster/blob/main/examples/ModbusRTUSlaveExample/README.md) for hardware setup details.
  
  See https://github.com/CMB27/ModbusADU for details on ModbusADU.
  See https://github.com/CMB27/ModbusSlaveLogic for details on ModbusSlaveLogic.
  See https://github.com/CMB27/ModbusRTUComm for details on ModbusRTUComm.
  
  Created: 2024-12-30
  By: C. M. Bulliner
  Last Modified: 2024-12-31
  By: C. M. Bulliner
  
*/

#include <ModbusADU.h>
#include <ModbusSlaveLogic.h>
#include <ModbusRTUComm.h>

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(ARDUINO_SAM_DUE)
  // The ATmega328P and ATmega168 are used in the Ardunino UNO and similar boards.
  // The ATmega2560 and ATmega1280 are used in the Arduino Mega and similar.
  #define MODBUS_SERIAL Serial
#elif defined(ARDUINO_NANO_ESP32)
  // On the Arduino Nano ESP32, the HardwareSerial port on pins 0 and 1 is Serial0.
  #define MODBUS_SERIAL Serial0
#else
  // On the majority of Arduino boards, the HardwareSerial port on pins 0 and 1 is Serial1.
  // On the Arduino Mega and Adruino Due, Serial1 is on pins 18 and 19.
  #define MODBUS_SERIAL Serial1
#endif
// You can change the baud, config, and unit id values if you like.
// Just make sure they match the settings you use in ModbusRTUMasterExample.
#define MODBUS_BAUD 38400
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1

#if (defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_ARCH_MBED)) || defined(ARDUINO_NANO_ESP32)
  // These boards operate unsing GPIO numbers that don't correspond to the numbers on the boards.
  // However they do have D# values #defined to correct this.
  const int8_t buttonPins[2] = {D2, D3};
  const int8_t ledPins[4] = {D5, D6, D7, D8};
  const int8_t dePin = D13;
#else
  // Other boards do not have D# values, and will throw an error if you try to use them.
  const int8_t buttonPins[2] = {2, 3};
  const int8_t ledPins[4] = {5, 6, 7, 8};
  const int8_t dePin = 13;
#endif
const int8_t knobPins[2] = {A0, A1};

ModbusRTUComm rtuComm(MODBUS_SERIAL, dePin);
ModbusSlaveLogic modbusLogic;

const uint8_t numCoils = 2;
const uint8_t numDiscreteInputs = 2;
const uint8_t numHoldingRegisters = 2;
const uint8_t numInputRegisters = 2;

bool coils[numCoils];
bool discreteInputs[numDiscreteInputs];
uint16_t holdingRegisters[numHoldingRegisters];
uint16_t inputRegisters[numInputRegisters];



void setup() {
  pinMode(knobPins[0], INPUT);
  pinMode(knobPins[1], INPUT);
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  #if defined(ARDUINO_NANO_ESP32)
    analogReadResolution(10);
  #endif

  modbusLogic.configureCoils(coils, numCoils);
  modbusLogic.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbusLogic.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbusLogic.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
  rtuComm.begin(MODBUS_BAUD, MODBUS_CONFIG);
}

void loop() {
  inputRegisters[0] = map(analogRead(knobPins[0]), 0, 1023, 0, 255);
  inputRegisters[1] = map(analogRead(knobPins[1]), 0, 1023, 0, 255);
  discreteInputs[0] = !digitalRead(buttonPins[0]);
  discreteInputs[1] = !digitalRead(buttonPins[1]);

  ModbusADU adu;                                        // creates a ModbusADU object to hold the modbus data frame
  uint8_t error = rtuComm.readAdu(adu);                 // reads any available data and checks for errors
  if (error) return;                                    // skips the rest of loop if there are any errors
  uint8_t unitId = adu.getUnitId();                     // gets the unit id from the modbus data frame
  if (unitId != MODBUS_UNIT_ID && unitId != 0) return;  // skips the rest of loop if the data frame is not addressed to us and is not a broadcast message
  modbusLogic.processPdu(adu);                          // processes the data frame, possibly updating the modbus data arrays and formulating the response
  if (unitId != 0) rtuComm.writeAdu(adu);               // send the response if the request was not a broadcast message

  analogWrite(ledPins[0], holdingRegisters[0]);
  analogWrite(ledPins[1], holdingRegisters[1]);
  digitalWrite(ledPins[2], coils[0]);
  digitalWrite(ledPins[3], coils[1]);
}