/*
  ModbusRTUSlave Library - Holding Registers
  
  This sketch demonstrates how to setup ModbusRTUSlave with 2 holding registers.
  This sketch requires a Modbus RTU master/client device with an RS-232 or UART/USART port.
  If the port is a RS-232 port, a RS-232 to UART converter will be needed.
  If the port is UART but the logic levels of the port are different than the logic levels of your Arduino, a logic level converter will be needed.
  
  The master/client port will need to be cofigured using the following settings:
  - Baud Rate: 38400
  - Data Bits: 8
  - Parity: None
  - Stop Bit(s): 1
  - Slave/Server ID: 1
  - Holding Registers: 0 and 1
  
  Circuit:
  - Piezo speaker from pin 8 to GND
  - LED from pin 9 to GND with appropriate series resistor
  - TX of master/client to pin 10
  - RX of master/client to pin 11
  
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

const unsigned int numHoldingRegisters = 2;

const byte tonePin = 8;
const byte pwmPin = 9;
const byte rxPin = 10;
const byte txPin = 11;

byte buf[bufSize];
SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, buf, bufSize);

byte dutyCycle = 0;
boolean toneActive = true;
unsigned int toneFrequency = 0;


// This is a function that will be passed to the ModbusRTUSlave for reading holding registers.
long holdingRegisterRead(unsigned int address) {
  switch (address) {
    case 0:
      return dutyCycle;
      // No "break" is needed because we exited the function with "return".
    case 1:
      return toneFrequency;
  }
  // -1 can be returned if there is an error in getting a holding register value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

// This is a function that will be passed to the ModbusRTUSlave for writing to holding registers.
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
  // false or 0 can be returned if there is an error in setting a holding register value.
  // This will cause the library to send the appropriate exception response to the master/client device.
}

void setup() {
  pinMode(tonePin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  
  mySerial.begin(baud);
  modbus.begin(id, baud);
  modbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
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
