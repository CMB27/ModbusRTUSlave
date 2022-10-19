/*
  ModbusRTUSlave Library - Coil_HardwareSerial

  This sketch demonstrates how to setup a ModbusRTUSlave to control an LED using a HardwareSerial port.
  This test will require a Modbus RTU master device.
  Common examples of master devices are industrial PLCs and HMIs.
  These will typically communicate using RS-232 or RS-485.
  The logic levels of the master device will need to be at or converted to TTL levels for your Arduino.
  
  Circuit:
  - TX of master to pin 0
  - RX of master to pin 1

  Created: 2020-06-30
  By: C. M. Bulliner
  Modified: 2022-10-18
  By: C. M. Bulliner

*/

#include <ModbusRTUSlave.h>

// Here are some constants used elsewhere in the program.
const byte ledPin = LED_BUILTIN, id = 1, serialConfig = SERIAL_8E1;
const word bufSize = 256, numCoils = 1, ledAddress = 0;
const unsigned long baud = 38400;

// This is the buffer for the ModbusRTUSlave object.
// It is used to store the Modbus messages.
// A size of 256 bytes is recommended, but sizes as low as 8 bytes can be used.
byte buf[bufSize];

// Initilize a ModbusRTUSlave.
ModbusRTUSlave modbus(Serial, buf, bufSize);

// This is a function that will be passed to the ModbusRTUSlave for reading coils.
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
  Serial.begin(baud, serialConfig);

  // Setup the ModbusRTUSlave
  modbus.begin(id, baud, serialConfig);

  // Configure the coil(s).
  modbus.configureCoils(numCoils, coilRead, coilWrite);
}

void loop() {
  // Poll for Modbus RTU requests from the master device.
  // This will automatically run the coilRead or coilWrite functions as needed.
  modbus.poll();
}
