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

const byte button1Pin = 2;
const byte button2Pin = 3;
const byte tonePin = 8;
const byte pwmPin = 9;
const byte rxPin = 10;
const byte txPin = 11;
const byte ledPin = 13;
const byte pot1Pin = A0;
const byte pot2Pin = A1;

const unsigned int numCoils = 2;
const unsigned int numDiscreteInputs = 2;
const unsigned int numHoldingRegisters = 2;
const unsigned int numInputRegisters = 2;

bool coils[numCoils];
bool discreteInputs[numDiscreteInputs];
unsigned int holdingRegisters[numHoldingRegisters];
unsigned int inputRegisters[numInputRegisters];

SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial); // serial port, driver enable pin (optional)

byte dutyCycle = 0;
boolean toneActive = 0;
unsigned int toneFrequency = 0;

void setup() {
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(pwmPin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);

  modbus.configureCoils(coils, numCoils);
  modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);
  modbus.begin(1, 38400); // modbus slave id/address, baud rate, config (optional)
  
}

void loop() {
  discreteInputs[0] = !digitalRead(button1Pin);
  discreteInputs[1] = !digitalRead(button2Pin);
  inputRegisters[0] = analogRead(pot1Pin);
  inputRegisters[1] = analogRead(pot2Pin);
  
  modbus.poll();
  
  digitalWrite(ledPin, coils[0]);
  if (holdingRegisters[0] > 255) analogWrite(pwmPin, 255);
  else analogWrite(pwmPin, holdingRegisters[0]);
  if (coils[1] and holdingRegisters[1] > 31) tone(tonePin, holdingRegisters[1]);
  else noTone(tonePin);
}
