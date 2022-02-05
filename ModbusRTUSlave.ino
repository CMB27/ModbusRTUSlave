#include "ModbusRTUSlave.h"

const byte buttonPin = 12, ledPin = 13, potPin = A0;

const size_t bufLen = 256, numCoils = 8, numInputs = 8, numHRegs = 4, numInRegs = 4;
const byte id = 1;
const unsigned long baud = 38400;
byte buf[bufLen];
bool coils[numCoils], inputs[numInputs];
unsigned int hRegs[numHRegs], inRegs[numInRegs];
ModbusRTUSlave modbus(Serial1, buf, bufLen, coils, numCoils, inputs, numInputs, hRegs, numHRegs, inRegs, numInRegs);

unsigned int hReg0Prev;

void setup() {
  Serial.begin(9600);
  while(millis() < 2000 and !Serial);
  Serial.println(hRegs[0]);
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  Serial1.begin(baud);
  modbus.begin(id, baud);

}

void loop() {
  inputs[0] = !digitalRead(buttonPin);
  inRegs[0] = analogRead(potPin);

  modbus.poll();

  digitalWrite(ledPin, coils[0]);
  if (hRegs[0] != hReg0Prev) {
    hReg0Prev = hRegs[0];
    Serial.println(hRegs[0]);
  }
  
}
