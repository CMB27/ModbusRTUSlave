#include "ModbusRTUSlave.h"

const byte buttonPin = 12, led1Pin = 13,  led2Pin = 11, voltagePin = A0;

const word bufSize = 256, numCoils = 1, numDiscreteInputs = 1, numHoldingRegisters = 1, numInputRegisters = 1;
const byte id = 1;
const unsigned long baud = 38400;
byte buf[bufSize];

ModbusRTUSlave modbus(Serial1, buf, bufSize);

word led2Value;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(voltagePin, INPUT);
  modbus.configureCoils(numCoils, coilRead, coilWrite);
  modbus.configureDiscreteInputs(numDiscreteInputs, discreteInputRead);
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
  Serial1.begin(baud);
  modbus.begin(id, baud);
}

void loop() {
  modbus.poll();
}

char coilRead(word address) {
  if (address == 0) return digitalRead(led1Pin);
  else return false;
}

boolean coilWrite(word address, boolean value) {
  if (address == 0) digitalWrite(led1Pin, value);
  return true;
}

char discreteInputRead(word address) {
  if (address == 0) return !digitalRead(buttonPin);
  else return false;
}

long holdingRegisterRead(word address) {
  if (address == 0) return led2Value;
  else return 0;
}

boolean holdingRegisterWrite(word address, word value) {
  if (address == 0) {
    led2Value = value;
    analogWrite(led2Pin, map(led2Value, 0, 100, 0, 255));
  }
  return true;
}

long inputRegisterRead(word address) {
  if (address == 0) return map(analogRead(voltagePin), 0, 1023, 0, 5000);
  else return 0;
}
