/*
  ModbusRTUSlaveExample

  This example demonstrates how to setup and use the ModbusRTUSlave library (https://github.com/CMB27/ModbusRTUSlave).
  It is intended to be used with a second board running ModbusRTUMasterExample from the ModbusRTUMaster library (https://github.com/CMB27/ModbusRTUMaster).

  This program has been succsessfully tested with the following boards:
  - Arduino Leonardo
  - Arduino Make Your UNO (USB had to be unplugged to work with HardwareSerial)
  - Arduino Mega 2560
  - Arduino Nano
  - Arduino Nano 33 BLE
  - Arduino Nano 33 IoT
  - Arduino Nano ESP32
  - Arduino Nano Every
  - Arduino Nano RP2040 Connect - Using Earle F. Philhower's arduino-pico core
  - Arduino UNO R3 SMD
  - Arduino UNO R4

  Problems were encountered with the following board:
  - Arduino Nano RP2040 Connect - Using Arduino's ArduinoCore-mbed (Reliable communication could not be established with the master/client board)

  !WARNING
  When connecting boards using UART, as described in the circuit below, the logic level voltages must match (5V or 3.3V).
  If they do not, use a logic level converter, otherwise your 3.3V board could be damaged.
  
  Circuit:
  - The center pin of a potentiometer to pin A0, and the outside pins of the potentiometer to your board's logic level voltage (5V or 3.3V) and GND
  - The center pin of a potentiometer to pin A1, and the outside pins of the potentiometer to your board's logic level voltage (5V or 3.3V) and GND
  - A pushbutton switch from pin 2 to GND
  - A pushbutton switch from pin 3 to GND
  - A LED from pin 5 to GND with a 1K ohm series resistor
  - A LED from pin 6 to GND with a 1K ohm series resistor
  - A LED from pin 7 to GND with a 1K ohm series resistor
  - A LED from pin 8 to GND with a 1K ohm series resistor
  - RX pin (typically pin 0 or pin 10 if using SoftwareSerial) to TX pin of the master/client board
  - TX pin (typically pin 1 or pin 11 if using SoftwareSerial) to RX pin of the master/client board
  - GND to GND of the master/client board
  - Pin 13 is set up as the driver enable pin. This pin will be HIGH whenever the board is transmitting.

  !NOTE
  Both boards will also need power.
  
  Created: 2023-07-22
  By: C. M. Bulliner
  Last Modified: 2024-01-27
  By: C. M. Bulliner
  
*/

#if __AVR__
  // Uncomment the following line if you want to use SoftwareSerial on pins 10 and ll; note this only works on AVR boards.
  //# define USE_SOFTWARE_SERIAL
#endif

#if defined USE_SOFTWARE_SERIAL
  #include <SoftwareSerial.h>
#endif

#include <ModbusRTUSlave.h>

const byte potPins[2] = {A0, A1};
#if defined ESP32 || (defined ARDUINO_NANO_RP2040_CONNECT && !defined ARDUINO_ARCH_MBED)
const byte buttonPins[2] = {D2, D3};
const byte ledPins[4] = {D5, D6, D7, D8};
const byte dePin = D13;
#else
const byte buttonPins[2] = {2, 3};
const byte ledPins[4] = {5, 6, 7, 8};
const byte dePin = 13;
#endif

#if defined USE_SOFTWARE_SERIAL
  const byte rxPin = 10;
  const byte txPin = 11;
  SoftwareSerial mySerial(rxPin, txPin);
  ModbusRTUSlave modbus(mySerial, dePin);  // serial port, driver enable pin for rs-485
#else
  #if (defined __AVR_ATmega328P__ || defined __AVR_ATmega168__ || defined __AVR_ATmega1280__ || defined __AVR_ATmega2560__)
    ModbusRTUSlave modbus(Serial, dePin);  // serial port, driver enable pin for rs-485
  #elif defined ESP32
    ModbusRTUSlave modbus(Serial0, dePin); // serial port, driver enable pin for rs-485
  #else
    ModbusRTUSlave modbus(Serial1, dePin); // serial port, driver enable pin for rs-485
  #endif
#endif

bool coils[2];
bool discreteInputs[2];
uint16_t holdingRegisters[2];
uint16_t inputRegisters[2];

void setup() {
  #if defined ESP32
    analogReadResolution(10);
  #endif

  pinMode(potPins[0], INPUT);
  pinMode(potPins[1], INPUT);
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  modbus.configureCoils(coils, 2);                       // bool array of coil values, number of coils
  modbus.configureDiscreteInputs(discreteInputs, 2);     // bool array of discrete input values, number of discrete inputs
  modbus.configureHoldingRegisters(holdingRegisters, 2); // unsigned 16 bit integer array of holding register values, number of holding registers
  modbus.configureInputRegisters(inputRegisters, 2);     // unsigned 16 bit integer array of input register values, number of input registers

  modbus.begin(1, 38400);
}

void loop() {
  inputRegisters[0] = map(analogRead(potPins[0]), 0, 1023, 0, 255);
  inputRegisters[1] = map(analogRead(potPins[1]), 0, 1023, 0, 255);
  discreteInputs[0] = !digitalRead(buttonPins[0]);
  discreteInputs[1] = !digitalRead(buttonPins[1]);
  
  modbus.poll();

  analogWrite(ledPins[0], holdingRegisters[0]);
  analogWrite(ledPins[1], holdingRegisters[1]);
  digitalWrite(ledPins[2], coils[0]);
  digitalWrite(ledPins[3], coils[1]);
}
