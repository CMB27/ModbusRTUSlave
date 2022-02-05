#include "ModbusRTUSlave.h"

const byte buttonPin = 12, ledPin = 13, potPin = A0;

const word numCoils = 8, numDiscreteInputs = 8, numHoldingRegisters = 4, numInputRegisters = 4;
const byte id = 1;
const unsigned long baud = 38400;

ModbusRTUSlave modbus(Serial1);

boolean coils[numCoils], discreteInputs[numDiscreteInputs];
word holdingRegisters[numHoldingRegisters], inputRegisters[numInputRegisters];
boolean coilRead(word address) {return coils[address];}
void coilWrite(word address, boolean value) {coils[address] = value;}
boolean discreteInputRead(word address) {return discreteInputs[address];}
word holdingRegisterRead(word address) {return holdingRegisters[address];}
void holdingRegisterWrite(word address, word value) {holdingRegisters[address] = value;}
word inputRegisterRead(word address) {return inputRegisters[address];}

word holdingRegisterOld;

void setup() {
  Serial.begin(9600);
  while(millis() < 2000 and !Serial);
  Serial.println(holdingRegisters[0]);
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  modbus.configureCoils(numCoils, coilRead, coilWrite);
  modbus.configureDiscreteInputs(numDiscreteInputs, discreteInputRead);
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
  Serial1.begin(baud);
  modbus.begin(id, baud);

}

void loop() {
  discreteInputs[0] = !digitalRead(buttonPin);
  inputRegisters[0] = analogRead(potPin);

  modbus.poll();

  digitalWrite(ledPin, coils[0]);
  if (holdingRegisters[0] != holdingRegisterOld) {
    holdingRegisterOld = holdingRegisters[0];
    Serial.println(holdingRegisters[0]);
  }
  
}
