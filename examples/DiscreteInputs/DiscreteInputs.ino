/*
  ModbusRTUSlave Library - DiscreteInputs
  
  This sketch demonstrates how to setup ModbusRTUSlave with 2 discrete inputs.
  This sketch requires a Modbus RTU master/client device with an RS-232 or UART/USART port.
  If the port is a RS-232 port, a RS-232 to UART converter will be needed.
  If the port is UART but the logic levels of the port are different than the logic levels of your Arduino, a logic level converter will be needed.
  
  The master/client will need to be cofigured using the following settings:
  - Baud Rate: 38400
  - Data Bits: 8
  - Parity: None
  - Stop Bit(s): 1
  - Slave/Server ID: 1
  - Discrete Inputs: 0 and 1
  
  Circuit:
  - A pushbutton switch from pin 2 to GND
  - A pushbutton switch from pin 3 to GND
  - TX of master/client to pin 10
  - RX of master/client to pin 11
  
  Created: 2022-11-19
  By: C. M. Bulliner

*/

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

const byte id = 1;
const unsigned long baud = 38400;
const unsigned int bufSize = 256;

const unsigned int numDiscreteInputs = 2;

const byte buttonPins[numDiscreteInputs] = {2, 3};
const byte rxPin = 10;
const byte txPin = 11;

byte buf[bufSize];
SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, buf, bufSize);

// This is a function that will be passed to the ModbusRTUSlave for reading discrete inputs.
char discreteInputRead(unsigned int address) {
  return !digitalRead(buttonPins[address]);
  // -1 can be returned if there is an error in getting a discrete input value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

void setup() {
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  
  mySerial.begin(baud);
  modbus.begin(id, baud);
  modbus.configureDiscreteInputs(numDiscreteInputs, discreteInputRead);
}

void loop() {
  modbus.poll();
}

