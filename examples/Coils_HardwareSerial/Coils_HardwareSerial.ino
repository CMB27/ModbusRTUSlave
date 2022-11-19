/*
  ModbusRTUSlave Library - Coils_HardwareSerial
  
  This sketch demonstrates how to setup ModbusRTUSlave with 2 coils.
  This sketch requires a Modbus RTU master/client device with an RS-232 or UART/USART port.
  If the port is a RS-232 port, a RS-232 to UART converter will be needed.
  If the port is UART but the logic levels of the port are different than the logic levels of your Arduino, a logic level converter will be needed.
  
  The master/client will need to be cofigured using the following settings:
  - Baud Rate: 38400
  - Data Bits: 8
  - Parity: Even
  - Stop Bit(s): 1
  - Slave/Server ID: 1
  - Coils: 0 and 1
  
  Circuit:
  - TX of master to pin 0
  - RX of master to pin 1
  - Piezo speaker from pin 8 to GND
  - LED from pin 13 to GND with appropriate series resistor (the built in LED will do)
  
  Created: 2022-11-19
  By: C. M. Bulliner
  
*/

#include <ModbusRTUSlave.h>

const byte id = 1;
const unsigned long baud = 38400;
const byte config = SERIAL_8E1;
const unsigned int bufSize = 256;

const unsigned int numCoils = 2;

const byte tonePin = 8;
const byte ledPin = 13;

byte buf[bufSize];
ModbusRTUSlave modbus(Serial, buf, bufSize);

boolean toneActive = 0;
unsigned int toneFrequency = 2000;

// This is a function that will be passed to ModbusRTUSlave for reading coils.
char coilRead(unsigned int address) {
  switch (address) {
    case 0:
      return digitalRead(ledPin);
      // No "break" is needed because we exited the function with "return".
    case 1:
      return toneActive;
  }
  // -1 can be returned if there is an error in getting a coil value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

// This is a function that will be passed to ModbusRTUSlave for writing to coils.
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
  // false or 0 can be returned if there is an error in setting a coil value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

void setup() {
  pinMode(tonePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(baud, config);
  modbus.begin(id, baud, config);
  modbus.configureCoils(numCoils, coilRead, coilWrite);
}

void loop() {
  modbus.poll();
  if (toneActive == true and toneFrequency >= 31) {
    tone(tonePin, toneFrequency);
  }
  else {
    noTone(tonePin);
  }
}
