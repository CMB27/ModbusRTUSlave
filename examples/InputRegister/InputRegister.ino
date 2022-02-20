/*
  ModbusRTUSlave Library - InputRegister

  This sketch demonstrates how to setup a ModbusRTUSlave to communicate the position of a potentiometer.
  This test will require a Modbus RTU master device.
  Common examples of master devices are industrial PLCs and HMIs.
  These will typically communicate using RS-232 or RS-485.
  The logic levels of the master device will need to be at or converted to TTL levels for your Arduino.
  
  Circuit:
  - TX of master to pin 10
  - RX of master to pin 11
  - Potentiometer center pin to A0, side pin to GND, and other side pin to +5V

  Created: 2020-02-19
  By: C. M. Bulliner

*/

// ModbusRTUSlave is not dependant on SoftwareSerial.
// It is included here so that the Serial port can be kept free for debugging.
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

// Here are some constants used elsewhere in the program.
const byte sensorPin = A0, rxPin = 10, txPin = 11, id = 1;
const word bufSize = 256, numInputRegisters = 1, sensorAddress = 0;
const unsigned long baud = 38400;

// This is the buffer for the ModbusRTUSlave object.
// It is used to store the Modbus messages.
// A size of 256 bytes is recommended, but sizes as low as 8 bytes can be used.
byte buf[bufSize];

// Initilize a SoftwareSerial port.
SoftwareSerial mySerial(rxPin, txPin);

// Initilize a ModbusRTUSlave.
ModbusRTUSlave modbus(mySerial, buf, bufSize);

// This is a funciton that will be passed to the ModbusRTUSlave for reading input registers.
long inputRegisterRead(word address) {
  if (address == sensorAddress) return analogRead(sensorPin);
  else return false;
}

void setup() {
  // Setup the potentiometer pin.
  pinMode(sensorPin, INPUT);

  // Setup the SoftwareSerial port.
  mySerial.begin(baud);

  // Setup the ModbusRTUSlave
  modbus.begin(id, baud);

  // Configure the inputRegister(s).
  modbus.configureInputRegisters(numInputRegisters, inputRegisterRead);
}

void loop() {
  // Poll for Modbus RTU requests from the master device.
  // This will autmatically run the inputRegisterRead function as needed.
  modbus.poll();
}
