# ModbusRTUSlave
Modbus is an industrial communication protocol. The RTU variant communicates over serial lines such as UART, RS-232, or RS-485. The full details of the Modbus protocol can be found at [modbus.org](https://modbus.org). A good summary can also be found on [Wikipedia](https://en.wikipedia.org/wiki/Modbus).

This is an Arduino library that implements the slave/server logic of the Modbus RTU protocol. This library implements function codes 1 (Read Coils), 2 (Read Discrete Inputs), 3 (Read Holding Registers), 4 (Read Input Registers), 5 (Write Single Coil), 6 (Write Single Holding Register), 15 (Write Multiple Coils), and 16 (Write Multiple Holding Registers).

This library will work with HardwareSerial, SoftwareSerial, or Serial_ (USB Serial on ATmega32u4 based boards). A driver enable pin can be set, enabling an RS-485 transceiver to be used. This library requires arrays for coils, discrete inputs, holding registers, and input registers to be passed to it. 


## Version Note
Version 2.x.x of this library is not backward compatible with version 1.x.x. Any sketches that were written to use a 1.x.x version of this library will not work with later versions, at least not without modification.


## Example
- [ModbusRTUSlaveExample](./extras/ModbusRTUSlaveExample.md)


## Methods


### `ModbusRTUSlave()`

#### Syntax
``` C++
ModbusRTUSlave(serial)
ModbusRTUSlave(serial, dePin)
```

#### Parameters
- `serial`: the serial port object to use for Modbus communication.
- `dePin`: the driver enable pin. This pin is set HIGH when transmitting. If this parameter is set to `NO_DE_PIN`, this feature will be disabled. Default value is `NO_DE_PIN`. Allowed data types `uint8_t` or `byte`.

---


### `configureCoils()`

#### Syntax
``` C++
modbus.configureCoils(coils, numCoils)
```

#### Parameters
- `coils`: an array of coil values. Allowed data types: array of `bool`.
- `numCoils`: the number of coils. This value must not be larger than the size of the array. Allowed data types `uint16_t`.

---


### `configureDiscreteInputs()`

#### Syntax
``` C++
modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs)
```

#### Parameters
- `discreteInputs`: an array of discrete input values. Allowed data types: array of `bool`.
- `numDiscreteInputs`: the number of discrete inputs. This value must not be larger than the size of the array. Allowed data types `uint16_t`.

---



### `configureHoldingRegisters()`

#### Syntax
``` C++
modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters)
```

#### Parameters
- `holdingRegisters`: an array of holding register values. Allowed data types: array of `uint16_t`.
- `numHoldingRegisters`: the number of holding registers. This value must not be larger than the size of the array. Allowed data types `uint16_t`.

---


### `configureInputRegisters()`

#### Syntax
``` C++
modbus.configureInputRegisters(inputRegisters, numInputRegisters)
```

#### Parameters
- `inputRegisters`: an array of input register values. Allowed data types: array of `uint16_t`.
- `numInputRegisters`: the number of input registers. This value must not be larger than the size of the array. Allowed data types `uint16_t`.

---


### `begin()`

#### Syntax
``` C++
modbus.begin(slaveId, baud)
modbus.begin(slaveId, baud, config)
```

#### Parameters
- `slaveId`: the number used to itentify this device on the Modbus network. Allowed data types `uint8_t` or `byte`.
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


### `poll()`

#### Syntax
``` C++
poll()
```

#### Parameters
None