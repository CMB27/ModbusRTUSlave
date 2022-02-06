#include "ModbusRTUSlave.h"

const byte buttonPin = 12, ledPin = 13, voltagePin = A0;

const word bufSize = 256, numCoils = 8, numDiscreteInputs = 8, numHoldingRegisters = 4, numInputRegisters = 4;
const byte id = 1;
const unsigned long baud = 38400;
byte buf[bufSize];

ModbusRTUSlave modbus(Serial1, buf, bufSize);

boolean coils[numCoils], discreteInputs[numDiscreteInputs];
word holdingRegisters[numHoldingRegisters], inputRegisters[numInputRegisters];

word holdingRegisterOld;

void setup() {
  
  Serial.begin(9600);
  while(millis() < 2000 and !Serial);
  Serial.println(holdingRegisters[0]);
  
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(voltagePin, INPUT);

  modbus.configureCoils(numCoils, coilRead, coilWrite);
  modbus.configureDiscreteInputs(numDiscreteInputs, discreteInputRead);
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
  Serial1.begin(baud);
  modbus.begin(id, baud);
}

void loop() {
  discreteInputs[0] = !digitalRead(buttonPin);
  inputRegisters[0] = analogRead(voltagePin);

  modbus.poll();

  digitalWrite(ledPin, coils[0]);
  if (holdingRegisters[0] != holdingRegisterOld) {
    holdingRegisterOld = holdingRegisters[0];
    Serial.println(holdingRegisters[0]);
  }
}

char coilRead(word address) {
  return coils[address];
}

boolean coilWrite(word address, boolean value) {
  coils[address] = value;
  return true;
}

char discreteInputRead(word address) {
  return discreteInputs[address];
}

long holdingRegisterRead(word address) {
  return holdingRegisters[address];
}

boolean holdingRegisterWrite(word address, word value) {
  holdingRegisters[address] = value;
  return true;
}

long inputRegisterRead(word address) {
  return inputRegisters[address];
}
