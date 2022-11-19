/*
  ModbusRTUSlave Library - Input Registers
  
  This sketch demonstrates how to setup ModbusRTUSlave with 2 input registers.
  This sketch requires a Modbus RTU master/client device with an RS-232 or UART/USART port.
  If the port is a RS-232 port, a RS-232 to UART converter will be needed.
  If the port is UART but the logic levels of the port are different than the logic levels of your Arduino, a logic level converter will be needed.
  
  The master/client port will need to be cofigured using the following settings:
  - Baud Rate: 38400
  - Data Bits: 8
  - Parity: None
  - Stop Bit(s): 1
  - Slave/Server ID: 1
  - Input Registers: 0 and 1
  
  Circuit:
  - TX of master/client to pin 10
  - RX of master/client to pin 11
  - The center pin of a potentiometer to A0 with one end of the potentiometer connected to GND and the other to your Arduino's logic voltage (3.3 or 5V)
  - The center pin of a potentiometer to A1 with one end of the potentiometer connected to GND and the other to your Arduino's logic voltage (3.3 or 5V)
  
  Created: 2022-11-19
  By: C. M. Bulliner
  
*/

#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

const byte id = 1;
const unsigned long baud = 38400;
const unsigned int bufSize = 256;

const unsigned int numInputRegisters = 2;

const byte rxPin = 10;
const byte txPin = 11;
const byte potPins[numInputRegisters] = {A0, A1};

byte buf[bufSize];
SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, buf, bufSize);

// This is a function that will be passed to the ModbusRTUSlave for reading input registers.
long inputRegisterRead(word address) {
  return analogRead(potPins[address]);
  // -1 can be returned if there is an error in getting a discrete input value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

void setup() {
  pinMode(potPins[0], INPUT);
  pinMode(potPins[1], INPUT);
  
  mySerial.begin(baud);
  modbus.begin(id, baud);
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
}

void loop() {
  modbus.poll();
}
