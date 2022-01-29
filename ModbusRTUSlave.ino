#include "ModbusRTUSlave.h"
#include <LiquidCrystal.h>

const byte buttonPin = 10;
const byte ledPin = 13;
const byte potPin = A0;

const unsigned long baud = 38400;

const byte id = 1, numCoils = 8, numDiscreteInputs = 8, numHoldingRegisters = 4, numInputRegisters = 4;
ModbusRTUSlave modbus(id, numCoils, numDiscreteInputs, numHoldingRegisters, numInputRegisters);

const byte rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned int hRegPrev;

void setup() {
  
  lcd.begin(16, 2);
  lcd.print("hReg = ");

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);

  Serial.begin(baud);
  modbus.begin(Serial, baud);  

}

void loop() {
  modbus.discreteInputWrite(0, !digitalRead(buttonPin));
  modbus.inputRegisterWrite(0, analogRead(potPin));

  modbus.poll();

  digitalWrite(ledPin, modbus.coilRead(0));

  unsigned int hReg = modbus.holdingRegisterRead(0);
  if (hReg != hRegPrev) {
    hRegPrev = hReg;
    lcd.clear();
    lcd.print("hReg = ");
    lcd.print(hReg);
  }
  
}
