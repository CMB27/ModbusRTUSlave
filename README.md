# ModbusRTUSlave
ModbusRTUSlave is an Arduino library that allows an Arduino to communication with Modbus RTU master devices.
This library enables an Arduino board to be a Modbus RTU slave/server device, able to communicate with Modbus RTU master/client devices. This library will work with any Stream object such as HardwareSerial or SoftwareSerial. A driver enable pin can be set up, enabling the library to be used with a RS-485 transceiver. This library requires a few extra steps to set up, but this allows for a large degree of customization and optimization.
