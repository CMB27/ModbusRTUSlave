/*
  ModbusRTUSlave Library - Coil

  This sketch demonstrates how to setup a ModbusRTUSlave to control an LED.
  This test will require a Modbus RTU master device.
  Common examples of master devices are industrial PLCs and HMIs.
  These will typically communicate using RS-232 or RS-485.
  The logic levels of the master device will need to be at or converted to TTL levels for your Arduino.
  
  Circuit:
  - TX of master to pin 10
  - RX of master to pin 11
  - LED from pin 13 to GND, with appropriate series resistor

  Note: On most Arduino boards there is already an LED attached to pin 13.

  Created: 2020-02-13
  By: C. M. Bulliner
  Modified: 2020-02-19
  By: C. M. Bulliner

*/

// ModbusRTUSlave is not dependant on SoftwareSerial.
// It is included here so that the Serial port can be kept free for debugging.
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

// Here are some constants used elsewhere in the program.
const byte ledPin = 13, rxPin = 10, txPin = 11, id = 1;
const word bufSize = 256, numCoils = 1, ledAddress = 0;
const unsigned long baud = 38400;

// This is the buffer for the ModbusRTUSlave object.
// It is used to store the Modbus messages.
// A size of 256 bytes is recommended, but sizes as low as 8 bytes can be used.
byte buf[bufSize];

// Initilize a SoftwareSerial port.
SoftwareSerial mySerial(rxPin, txPin);

// Initilize a ModbusRTUSlave.
ModbusRTUSlave modbus(mySerial, buf, bufSize);

// This is a funciton that will be passed to the ModbusRTUSlave for reading coils.
char coilRead(word address) {
  if (address == ledAddress) return digitalRead(ledPin);
  else return false;
}

// This is a function that will be passed to the ModbusRTUSlave for writing to coils.
boolean coilWrite(word address, boolean value) {
  if (address == ledAddress) digitalWrite(ledPin, value);
  return true;
}

void setup() {
  // Setup the LED pin.
  pinMode(ledPin, OUTPUT);

  // Setup the SoftwareSerial port.
  mySerial.begin(baud);

  // Setup the ModbusRTUSlave
  modbus.begin(id, baud);

  // Configure the coil(s).
  modbus.configureCoils(numCoils, coilRead, coilWrite);
}

void loop() {
  // Poll for Modbus RTU requests from the master device.
  // This will autmatically run the coilRead or coilWrite functions as needed.
  modbus.poll();
}
