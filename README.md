# ModbusRTUSlave
Modbus is an industrial communication protocol. The RTU variant communicates over serial lines such as UART, RS-232, or RS-485. The full details of the Modbus protocol can be found at [modbus.org](https://modbus.org). A good summary can also be found on [Wikipedia](https://en.wikipedia.org/wiki/Modbus).

This is an Arduino library that implements the slave/server logic of the Modbus RTU protocol. This library implements function codes 1 (Read Coils), 2 (Read Discrete Inputs), 3 (Read Holding Registers), 4 (Read Input Registers), 5 (Write Single Coil), 6 (Write Single Holding Register), 15 (Write Multiple Coils), and 16 (Write Multiple Holding Registers).

This library will work with HardwareSerial, SoftwareSerial, or Serial_ (USB Serial on ATmega32u4 based boards). A driver enable pin can be set, enabling an RS-485 transceiver to be used. This library requires arrays for coils, discrete inputs, holding registers, and input registers to be passed to it. 


## Version Note
Version 2.x.x of this library is not backward compatible with version 1.x.x. Any sketches that were written to use a 1.x.x version of this library will not work with later versions, at least not without modification.


## Compatibility
This library has been succsessfully tested with the following boards:
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


## Example
- [ModbusRTUSlaveExample](https://github.com/CMB27/ModbusRTUSlave/blob/main/examples/ModbusRTUSlaveExample/ModbusRTUSlaveExample.ino)


## Methods

### ModbusRTUSlave()

#### Description
Creates a ModbusRTUSlave object and sets the serial port to use for data transmission.
Optionally sets a driver enable pin. This pin will go `HIGH` when the library is transmitting. This is primarily intended for use with an RS-485 transceiver, but it can also be a handy diagnostic when connected to an LED.

#### Syntax
``` C++
ModbusRTUSlave(serial)
ModbusRTUSlave(serial, dePin)
```

#### Parameters
- `serial`: the serial port object to use for Modbus communication.
- `dePin`: the driver enable pin. This pin is set HIGH when transmitting. If this parameter is set to `NO_DE_PIN`, this feature will be disabled. Default value is `NO_DE_PIN`. Allowed data types: `uint8_t` or `byte`.

#### Example
``` C++
# include <ModbusRTUSlave.h>

const uint8_t dePin = 13;

ModbusRTUSlave modbus(Serial, dePin);
```

---


### configureCoils()

#### Description
Tells the library where coil data is stored and the number of coils.
If this function is not run, the library will assume there are no coils.

#### Syntax
``` C++
modbus.configureCoils(coils, numCoils)
```

#### Parameters
- `coils`: an array of coil values. Allowed data types: array of `bool`.
- `numCoils`: the number of coils. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

---


### configureDiscreteInputs()

#### Description
Tells the library where to read discrete input data and the number of discrete inputs.
If this function is not run, the library will assume there are no discrete inputs.

#### Syntax
``` C++
modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs)
```

#### Parameters
- `discreteInputs`: an array of discrete input values. Allowed data types: array of `bool`.
- `numDiscreteInputs`: the number of discrete inputs. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

---



### configureHoldingRegisters()

#### Description
Tells the library where holding register data is stored and the number of holding registers.
If this function is not run, the library will assume there are no holding registers.

#### Syntax
``` C++
modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters)
```

#### Parameters
- `holdingRegisters`: an array of holding register values. Allowed data types: array of `uint16_t`.
- `numHoldingRegisters`: the number of holding registers. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

---


### configureInputRegisters()

#### Description
Tells the library where to read input register data and the number of input registers.
If this function is not run, the library will assume there are no input registers.

#### Syntax
``` C++
modbus.configureInputRegisters(inputRegisters, numInputRegisters)
```

#### Parameters
- `inputRegisters`: an array of input register values. Allowed data types: array of `uint16_t`.
- `numInputRegisters`: the number of input registers. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

---


### begin()

#### Description
Sets the slave/server id and the data rate in bits per second (baud) for serial transmission.
Optionally it also sets the data configuration. Note, there must be 8 data bits for Modbus RTU communication. The default configuration is 8 data bits, no parity, and one stop bit.

#### Syntax
``` C++
modbus.begin(slaveId, baud)
modbus.begin(slaveId, baud, config)
```

#### Parameters
- `slaveId`: the number used to itentify this device on the Modbus network. Allowed data types: `uint8_t` or `byte`.
- `baud`: the baud rate to use for Modbus communication. Common values are: `1200`, `2400`, `4800`, `9600`, `16200`, `38400`, `57600`, and `115200`. Allowed data types: `uint32_t`.
- `config`: the serial port configuration to use. Valid values are:  
`SERIAL_8N1`: no parity (default)  
`SERIAL_8N2`  
`SERIAL_8E1`: even parity  
`SERIAL_8E2`  
`SERIAL_8O1`: odd parity  
`SERIAL_8O2`

_If using a SoftwareSerial port a configuration of `SERIAL_8N1` will be used regardless of what is entered._

---


### poll()

#### Description
Checks if any Modbus requests are available. If a valid request has been received, an appropriate response will be sent.
This function must be called frequently.

#### Syntax
``` C++
modbus.poll()
```

#### Parameters
None

#### Example
``` C++
# include <ModbusRTUSlave.h>

const uint8_t coilPins[2] = {4, 5};
const uint8_t discreteInputPins[2] = {2, 3};

ModbusRTUSlave modbus(Serial);

bool coils[2];
bool discreteInputs[2];

void setup() {
  pinMode(coilPins[0], OUTPUT);
  pinMode(coilPins[1], OUTPUT);
  pinMode(discreteInputPins[0], INPUT);
  pinMode(discreteInputPins[1], INPUT);

  modbus.configureCoils(coils, 2);
  modbus.configureDiscreteInputs(discreteInputs, 2);
  modbus.begin(1, 38400);
}

void loop() {
  discreteInputs[0] = digitalRead(discreteInputPins[0]);
  discreteInputs[1] = digitalRead(discreteInputPins[1]);

  modbus.poll();

  digitalWrite(coilPins[0], coils[0]);
  digitalWrite(coilPins[1], coils[1]);
}

```