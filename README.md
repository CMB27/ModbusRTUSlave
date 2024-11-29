# ModbusRTUSlave

Modbus is an industrial communication protocol. The RTU variant communicates over serial lines such as UART, RS-232, or RS-485. The full details of the Modbus protocol can be found at [modbus.org](https://modbus.org). A good summary can also be found on [Wikipedia](https://en.wikipedia.org/wiki/Modbus).

This is an Arduino library that implements the slave/server logic of the Modbus RTU protocol. It enables an Arduino, or arduino compatible, board to respond to Modbus RTU requests from a Modbus master/client. This library implements function codes 1 (Read Coils), 2 (Read Discrete Inputs), 3 (Read Holding Registers), 4 (Read Input Registers), 5 (Write Single Coil), 6 (Write Single Holding Register), 15 (Write Multiple Coils), and 16 (Write Multiple Holding Registers).

This library will work with any `Stream` object, like `Serial`. A driver enable pin can be set up, enabling a half-duplex RS-485 transceiver to be used. Only `SERIAL_8N1`, `SERIAL_8E1`, `SERIAL_8O1`, `SERIAL_8N2`, `SERIAL_8E2`, and `SERIAL_8O2` configurations are supported; attempting to use any other configuration will cause the library to default to timings for `SERIAL_8N1`.

This library updates coil, descrete input, holding register, and input register arrays based on Modbus requests. It does not give indication of what has changed, or even if a valid Modbus reguest has been received. This is done to keep the library simple and easy to use.



## Version Note
### 1.x.x to 2.x.x
Version 2.x.x of this library is not backward compatible with version 1.x.x. Any sketches that were written to use a 1.x.x version of this library will not work with later versions, at least not without modification.

### 2.x.x to 3.x.x
The main change going from version 2.x.x to 3.x.x si that `begin()` for the Serial object used needs to be run before running `begin()` for the library itself, e.g.:
```C++
Serial1.begin(38400);
modbus.begin(38400);
```
This library is also now dependent on [ModbusADU](https://github.com/CMB27/ModbusADU) and [ModbusRTUComm](https://github.com/CMB27/ModbusRTUComm).



## Compatibility
This library has been tested with the following boards and cores:

| Board Name                  | Core                                                                 | Works    |
| :-------------------------- | :------------------------------------------------------------------- | :------: |
| Arduino Due                 | **Arduino SAM Boards (32-bits ARM Cortex-M3)** by Arduino `1.6.12`   | Yes      |
| Arduino Giga                | **Arduino Mbed OS GIGA Boards** by Arduino `4.1.5`                   | Yes      |
| Arduino Leonardo            | **Arduino AVR Boards** by Arduino `1.8.6`                            | Yes      |
| Arduino Make Your UNO       | **Arduino AVR Boards** by Arduino `1.8.6`                            | Yes [^1] |
| Arduino Mega 2560           | **Arduino AVR Boards** by Arduino `1.8.6`                            | Yes      |
| Arduino Nano                | **Arduino AVR Boards** by Arduino `1.8.6`                            | Yes      |
| Arduino Nano 33 BLE         | **Arduino Mbed OS Nano Boards** by Arduino `4.1.5`                   | Yes      |
| Arduino Nano 33 IoT         | **Arduino SAMD Boards (32-bits ARM Cortex-M0+)** by Arduino `1.8.14` | Yes      |
| Arduino Nano ESP32          | **Arduino ESP32 Boards** by Arduino `2.0.13`                         | Yes      |
| Arduino Nano ESP32          | **esp32** by Espressif Systems `3.0.5`                               | Yes      |
| Arduino Nano Every          | **Arduino megaAVR Boards** by Arduino `1.8.8`                        | Yes      |
| Arduino Nano Matter         | **Silicon Labs** by Silicon Labs `2.1.0`                             | No [^2]  |
| Arduino Nano RP2040 Connect | **Arduino Mbed OS Nano Boards** by Arduino `4.1.5`                   | No [^3]  |
| Arduino Nano RP2040 Connect | **Raspberry Pi Pico/RP2040** by Earle F. Philhower, III `4.1.1`      | Yes      |
| Arduino UNO R3 SMD          | **Arduino AVR Boards** by Arduino `1.8.6`                            | Yes      |
| Arduino UNO R4 Minima       | **Arduino UNO R4 Boards** by Arduino `1.2.2`                         | Yes      |

[^1]: **Arduino Make Your UNO**  
The example program does not work with this board when connected to a computer via USB.
However, it does work when it is powered through the barrel jack.

[^2]: **Arduino Nano RP2040 Connect**  
This board has trouble receiving Modbus messages when using the `Arduino Mbed OS Nano Boards` core by Arduino.  
It seems that there is some sort of timing issue.  
It can technically be made to work if you tell the library that it is operating at a lower baud rate than the serial port assigned to the library is actually operating at.
However, this would cause the library to operate with unknown timing tolerances, possibly well outside the Modbus specification.

[^3]: **Arduino Nano Matter**  
As of this writing (2024-09-07), `flush()` is not properly implemented with Serial on this board.  
ModbusRTUMaster depends on `flush()` to know when to set the DE and RE pins LOW after a message is sent.



## Example
- [ModbusRTUSlaveExample](https://github.com/CMB27/ModbusRTUSlave/blob/main/examples/ModbusRTUSlaveExample/ModbusRTUSlaveExample.ino)



## Methods



<details><summary id="modbusrtuslave-1"><strong>ModbusRTUSlave()</strong></summary>
  <blockquote>

### Description
Creates a ModbusRTUSlave object and sets the serial port to use for data transmission.
Optionally sets a driver enable pin. This pin will go `HIGH` when the library is transmitting. This is primarily intended for use with an RS-485 transceiver, but it can also be a handy diagnostic when connected to an LED.

### Syntax
- `ModbusRTUSlave(serial)`
- `ModbusRTUSlave(serial, dePin)`
- `ModbusRTUSlave(serial, dePin, rePin)`

### Parameters
- `serial`: the `Stream` object to use for Modbus communication. Usually something like `Serial1`.
- `dePin`: the driver enable pin. This pin is set HIGH when transmitting. If this parameter is set to `-1`, this feature will be disabled. The default value is `-1`. Allowed data types are `int8_t` or `char`.
- `rePin`: works exacly the same way as `dePin`. This option is included for compatibility with RS-485 shields like the [Arduino MKR 485 Shield](https://store.arduino.cc/products/arduino-mkr-485-shield).

### Example
``` C++
# include <ModbusRTUSlave.h>

const int8_t dePin = A6;
const int8_t rePin = A5;

ModbusRTUSlave modbus(Serial, dePin, rePin);
```

  </blockquote>
</details>



<details><summary id="configurecoils"><strong>configureCoils()</strong></summary>
  <blockquote>

### Description
Tells the library where coil data is stored and the number of coils.
If this function is not run, the library will assume there are no coils.

### Syntax
`modbus.configureCoils(coils, numCoils)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `coils`: an array of coil values. Allowed data types: array of `bool`.
- `numCoils`: the number of coils. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

  </blockquote>
</details>



<details><summary id="configurediscreteinputs"><strong>configureDiscreteInputs()</strong></summary>
  <blockquote>

### Description
Tells the library where to read discrete input data and the number of discrete inputs.
If this function is not run, the library will assume there are no discrete inputs.

### Syntax
`modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `discreteInputs`: an array of discrete input values. Allowed data types: array of `bool`.
- `numDiscreteInputs`: the number of discrete inputs. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

  </blockquote>
</details>



<details><summary id="configureholdingregisters"><strong>configureHoldingRegisters()</strong></summary>
  <blockquote>

### Description
Tells the library where holding register data is stored and the number of holding registers.
If this function is not run, the library will assume there are no holding registers.

### Syntax
`modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `holdingRegisters`: an array of holding register values. Allowed data types: array of `uint16_t`.
- `numHoldingRegisters`: the number of holding registers. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

  </blockquote>
</details>



<details><summary id="configureinputregisters"><strong>configureInputRegisters()</strong></summary>
  <blockquote>

### Description
Tells the library where to read input register data and the number of input registers.
If this function is not run, the library will assume there are no input registers.

### Syntax
`modbus.configureInputRegisters(inputRegisters, numInputRegisters)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `inputRegisters`: an array of input register values. Allowed data types: array of `uint16_t`.
- `numInputRegisters`: the number of input registers. This value must not be larger than the size of the array. Allowed data types: `uint16_t`.

  </blockquote>
</details>



<details><summary id="setresponsedelay"><strong>setResponseDelay()</strong></summary>
  <blockquote>

### Description
Sets an optional response delay (in ms) for the slave (default 0).
If set to a non-zero value, the slave will wait for the specified number of milliseconds before sending the response.
This may be useful if tight control over the dePin from the master is not possible. Adding a delay will allow the master enough time to stop transmitting and avoid issues with multiple drivers on the the physical pins.

### Syntax
`modbus.setResponseDelay(responseDelay)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `responseDelay`: number of milliseconds to wait before responding to requests. Allowed data types: `unsigned long`.

  </blockquote>
</details>



<details><summary id="begin"><strong>begin()</strong></summary>
  <blockquote>

### Description
Sets the slave/server id and the data rate in bits per second (baud) for serial transmission.
Optionally it also sets the data configuration. Note, there must be 8 data bits for Modbus RTU communication. The default configuration is 8 data bits, no parity, and one stop bit.

### Syntax
- `modbus.begin(slaveId, baud)`
- `modbus.begin(slaveId, baud, config)`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.
- `slaveId`: the number used to itentify this device on the Modbus network. Allowed data types: `uint8_t` or `byte`.
- `baud`: the baud rate to use for Modbus communication. Common values are: `1200`, `2400`, `4800`, `9600`, `16200`, `38400`, `57600`, and `115200`. Allowed data types: `unsigned long`.
- `config`: the serial port configuration to use. Valid values are:  
`SERIAL_8N1`: no parity (default)  
`SERIAL_8N2`  
`SERIAL_8E1`: even parity  
`SERIAL_8E2`  
`SERIAL_8O1`: odd parity  
`SERIAL_8O2`

  </blockquote>
</details>



<details><summary id="poll"><strong>poll()</strong></summary>
  <blockquote>

### Description
Checks if any Modbus requests are available.
If a valid write request has been received, it will update the appropriate data array, and send an acknowledgment response.
If a valid read request has been received, it will send a response with the requested data.
If an invalid request has been received, it will either respond with an exception response or not at all, as per the Modbus specification. 
This function should be called frequently.

### Syntax
`modbus.poll()`

### Parameters
- `modbus`: a `ModbusRTUSlave` object.

### Example
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
  Serial.begin(38400);
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

  </blockquote>
</details>

