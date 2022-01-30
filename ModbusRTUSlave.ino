#include "ModbusRTUSlave.h"
#include <LiquidCrystal.h>

const byte buttonPin = 12;
const byte ledPin = 13;
const byte potPin = A0;

const unsigned long baud = 38400;
const byte id = 1, bufLen = 256, numCoils = 8, numInputs = 8, numHRegs = 4, numInRegs = 4;
byte buf[bufLen];
bool coils[numCoils], inputs[numDiscreteInputs];
unsigned int hRegs[numHRegs], inRegs[numInRegs];
ModbusRTUSlave modbus(Serial1, buf, bufLen, coils, numCoils, inputs, numInputs, hRegs, numHRegs, inRegs, numInRegs);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("ready");
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  Serial1.begin(baud);
  modbus.begin(id, baud);

}

void loop() {
  inputs[0] = !digitalRead(buttonPin);
  inRegs[0] = analogRead(potPin);

  Serial.println("ok");
  modbus.poll();

  digitalWrite(ledPin, coils[0]);
  
}
