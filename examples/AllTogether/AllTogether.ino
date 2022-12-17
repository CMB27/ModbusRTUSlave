/*
  ModbusRTUSlave Library - AllTogether
  
  This sketch demonstrates how to setup ModbusRTUSlave with 2 coils, 2 discrete inputs, 2 holding registers, and 2 input registers.
  This sketch requires a Modbus RTU master/client device with an RS232 or UART/USART port.
  If the port is a RS-232 port, a RS-232 to UART converter will be needed.
  If the port is UART but the logic levels of the port are different than the logic levels of your Arduino, a logic level converter will be needed.
  
  The master/client port will need to be cofigured using the following settings:
  - Baud Rate: 38400
  - Data Bits: 8
  - Parity: None
  - Stop Bit(s): 1
  - Slave/Server ID: 1
  - Coils: 0 and 1
  - Discrete Inputs: 0 and 1
  - Holding Registers: 0 and 1
  - Input Registers: 0 and 1
  
  Circuit:
  - A pushbutton switch from pin 2 to GND
  - A pushbutton switch from pin 3 to GND
  - Piezo speaker from pin 8 to GND
  - LED from pin 9 to GND with appropriate series resistor
  - TX of master/client to pin 10
  - RX of master/client to pin 11
  - LED from pin 13 to GND with appropriate series resistor (the built in LED will do)
  - The center pin of a potentiometer to A0 with one end of the potentiometer connected to GND and the other to your Arduino's logic voltage (3.3 or 5V)
  - The center pin of a potentiometer to A1 with one end of the potentiometer connected to GND and the other to your Arduino's logic voltage (3.3 or 5V)
  
  Created: 2022-11-19
  By: C. M. Bulliner
  Modified: 2022-12-17
  By: C. M. Bulliner
  
*/

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

const byte id = 1;
const unsigned long baud = 38400;
const unsigned int bufSize = 256;

const unsigned int numCoils = 2;
const unsigned int numDiscreteInputs = 2;
const unsigned int numHoldingRegisters = 2;
const unsigned int numInputRegisters = 2;

const byte buttonPins[numDiscreteInputs] = {2, 3};
const byte tonePin = 8;
const byte pwmPin = 9;
const byte rxPin = 10;
const byte txPin = 11;
const byte ledPin = 13;
const byte potPins[numInputRegisters] = {A0, A1};

byte buf[bufSize];
SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, buf, bufSize);

byte dutyCycle = 0;
boolean toneActive = 0;
unsigned int toneFrequency = 0;

void setup() {
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(potPins[0], INPUT);
  pinMode(potPins[1], INPUT);
  
  mySerial.begin(baud);
  modbus.begin(id, baud);
  modbus.configureCoils(numCoils, coilRead, coilWrite);
  modbus.configureDiscreteInputs(numDiscreteInputs, discreteInputRead);
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
}

void loop() {
  modbus.poll();
  if (toneActive == true and toneFrequency > 0) {
    tone(tonePin, toneFrequency);
  }
  else {
    noTone(tonePin);
  }
}



char coilRead(unsigned int address) {
  switch (address) {
    case 0:
      return digitalRead(ledPin);
    case 1:
      return toneActive;
  }
}

boolean coilWrite(unsigned int address, boolean value) {
  switch (address) {
    case 0:
      digitalWrite(ledPin, value);
      break;
    case 1:
      toneActive = value;
      break;
  }
  return true;
}

char discreteInputRead(unsigned int address) {
  return !digitalRead(buttonPins[address]);
}

long holdingRegisterRead(unsigned int address) {
  switch (address) {
    case 0:
      return dutyCycle;
    case 1:
      return toneFrequency;
  }
}

boolean holdingRegisterWrite(word address, word value) {
  switch (address) {
    case 0:
      dutyCycle = constrain(value, 0, 255);
      analogWrite(pwmPin, dutyCycle);
      break;
    case 1:
      toneFrequency = 0;
      if (value >= 31) {
        toneFrequency = value;
      }
      break;
  }
  return true;
}

long inputRegisterRead(word address) {
  return analogRead(potPins[address]);
}
