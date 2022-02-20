/*
  ModbusRTUSlave Library - HoldingRegister

  This sketch demonstrates how to setup a ModbusRTUSlave to control the brightness of an LED.
  This test will require a Modbus RTU master device.
  Common examples of master devices are industrial PLCs and HMIs.
  These will typically communicate using RS-232 or RS-485.
  The logic levels of the master device will need to be at or converted to TTL levels for your Arduino.
  
  Circuit:
  - TX of master to pin 10
  - RX of master to pin 11
  - LED from pin 9 to GND, with appropriate series resistor

  Created: 2020-02-19
  By: C. M. Bulliner

*/

// ModbusRTUSlave is not dependant on SoftwareSerial.
// It is included here so that the Serial port can be kept free for debugging.
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

// Here are some constants used elsewhere in the program.
const byte ledPin = 9, rxPin = 10, txPin = 11, id = 1;
const word bufSize = 256, numHoldingRegisters = 1, ledAddress = 0;
const unsigned long baud = 38400;

// This is the buffer for the ModbusRTUSlave object.
// It is used to store the Modbus messages.
// A size of 256 bytes is recommended, but sizes as low as 8 bytes can be used.
byte buf[bufSize];

// This is the variable used to control the LED brightness.
byte ledValue = 0;

// Initilize a SoftwareSerial port.
SoftwareSerial mySerial(rxPin, txPin);

// Initilize a ModbusRTUSlave.
ModbusRTUSlave modbus(mySerial, buf, bufSize);

// This is a funciton that will be passed to the ModbusRTUSlave for reading holding registers.
long holdingRegisterRead(word address) {
  if (address == ledAddress) return ledValue;
  else return false;
}

// This is a function that will be passed to the ModbusRTUSlave for writing to holding registers.
boolean holdingRegisterWrite(word address, word value) {
  if (address == ledAddress) {
    ledValue = value;
    analogWrite(ledPin, ledValue);
  }
  return true;
}

void setup() {
  // Setup the LED pin.
  pinMode(ledPin, OUTPUT);

  // Setup the SoftwareSerial port.
  mySerial.begin(baud);

  // Setup the ModbusRTUSlave
  modbus.begin(id, baud);

  // Configure the holdingRegister(s).
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
}

void loop() {
  // Poll for Modbus RTU requests from the master device.
  // This will autmatically run the holdingRegisterRead or holdingRegisterWrite functions as needed.
  modbus.poll();
}
