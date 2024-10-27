/*
  ModbusRTUSlaveExample

  This example demonstrates how to setup and use the ModbusRTUSlave library (https://github.com/CMB27/ModbusRTUSlave).
  See README.md (https://github.com/CMB27/ModbusRTUMaster/blob/main/examples/ModbusRTUSlaveExample/README.md) for hardware setup details.
  
  Created: 2023-07-22
  By: C. M. Bulliner
  Last Modified: 2024-10-26
  By: C. M. Bulliner
  
*/

#include <ModbusRTUSlave.h>

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

ModbusRTUSlave modbus(MODBUS_SERIAL, dePin);

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

  modbus.configureCoils(coils, numCoils);
  modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);
}

void loop() {
  inputRegisters[0] = map(analogRead(knobPins[0]), 0, 1023, 0, 255);
  inputRegisters[1] = map(analogRead(knobPins[1]), 0, 1023, 0, 255);
  discreteInputs[0] = !digitalRead(buttonPins[0]);
  discreteInputs[1] = !digitalRead(buttonPins[1]);
  
  modbus.poll();

  analogWrite(ledPins[0], holdingRegisters[0]);
  analogWrite(ledPins[1], holdingRegisters[1]);
  digitalWrite(ledPins[2], coils[0]);
  digitalWrite(ledPins[3], coils[1]);
}
