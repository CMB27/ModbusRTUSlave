# ModbusRTUSlave
## Description  
This is an Arduino library that implements the slave/server logic of the Modbus RTU protocol. This library implements function codes 1 (Read Coils), 2 (Read Discrete Inputs), 3 (Read Holding Registers), 4 (Read Input Registers), 5 (Write Single Coil), 6 (Write Single Holding Register), 15 (Write Multiple Coils), and 16 (Write Multiple Holding Registers).  
## Important Version Note
Version 2.x.x of this library is not backward compatible with version 1.x.x. Any sketches that were written to use a 1.x.x version of this library will not work with later versions, at least not without modification.  
## Modbus  
Modbus is an industrial communication protocol. The RTU variant communicates over serial lines such as RS-232 or RS-485. The full details of the Modbus protocol can be found at [modbus.org](https://modbus.org). A good summary can also be found on [Wikipedia](https://en.wikipedia.org/wiki/Modbus).  
## Library Details  
This library will work with HardwareSerial, SoftwareSerial, or Serial_ (USB Serial on ATmega32u4 based boards). A driver enable pin can be set up, enabling an RS-485 transceiver to be used. This library also requires arrays for coils, discrete inputs, holding registers, and input registers to be passed to it.  
