# ModbusRTUSlaveExample
 
This example demonstrates how to setup and use the [ModbusRTUSlave](https://github.com/CMB27/ModbusRTUSlave) library.

> [!IMPORTANT]  
> This is a communications library, so in order to test it, you will need something to communicate with.  
> A second board running ModbusRTUMasterExample from the [ModbusRTUMaster](https://github.com/CMB27/ModbusRTUMaster) library will be needed.  
> This second board will also need to be setup with the [circuit](#circuit) shown below.  


## Circuit:  
```
                                                 VCC                                                                 0.1µF Capacitor
                                                  ^                            Arduino Board                        /
                                                  |                           /                                  | |
 +--------------------o---------------------------o--------------------------/-------------------------------o---| |---+
 |                    |                                                     /                                |   | |   |
 |                    |         +------+                   +------+        /            RS-485               |         |
 |                    |      +--|      |-------------------|      |-------+        Transceiver               |         |
 |                    |      |  |      |                   +------+       |                    \             |         |               120 Ω Resistor
 |                    |      |  |      |                                  |              +-------v-------+   |         |        _____ /
 |                    |      |  +------+                          SCL [ ] |      +-------| RO        VCC |---+         |   +---|_____|---+
 |                    |      |                                    SDA [ ] |      |       |               |             |   |             |
 |                    |      |                                   AREF [ ] |      |   +---| RE          B |-------------|---o-------------|---<> RS485_D-
 |                    |      |                                    GND [ ] |      |   |   |               |             |                 |
 |                    |      | [ ] BOOT                            13 [ ] |------|---o---| DE          A |-------------|-----------------o---<> RS485_D+
 |                    +------| [ ] IOREF                           12 [ ] |      |       |               |             |
 |                           | [ ] RESET                          ~11 [ ] |      |   +---| DI        GND |-------------o
 |                           | [ ] 3.3V                           ~10 [ ] |      |   |   +---------------+             |
 |                           | [ ] 5V                              ~9 [ ] |      |   |                       _____     |
 |                           | [ ] GND                              8 [ ] |------|---|----------------->|---|_____|----o
 |                    +------| [ ] GND                                    |      |   |                       _____     |
 |                    |      | [ ] VIN                              7 [ ] |------|---|----------------->|---|_____|----o
 |                    |      |                                     ~6 [ ] |------|---|------------+                    |  4x LEDs
 |      +--------------------| [ ] A0                              ~5 [ ] |------|---|---------+  |          _____     |  4x 1K Ω Resistors
 |      |      +-------------| [ ] A1                               4 [ ] |      |   |         |  +---->|---|_____|----o
 |    __v__    |      |      | [ ] A2                              ~3 [ ] |------|---|------+  |             _____     |
 o---|_____|---|------o      | [ ] A3                               2 [ ] |------|---|---+  |  +------->|---|_____|----o
 |             |      |      | [ ] A4                            TX→1 [ ] |------|---+   |  |                          |
 |           __v__    |      | [ ] A5                            RX←0 [ ] |------+       |  |                          |
 +----------|_____|---o      |                                 __________/               |  |             __|__        |
                      |       \_______________________________/                          |  +-------------o   o--------o
       2x 10K Ω       |                                                                  |                             |  2x Pushbutton Switches
    Potentiometers    |                                                                  |                __|__        |
                      |                                                                  +----------------o   o--------o
                      |                                                                                                |
                      +------------------------------------------------------------------------------------------------o---------------------<> GND
```

**Components:**  
- Arduino Board
- Half-Duplex RS-485 Transceiver (Must be able to operate at your arduino board's logic level)
- 120 Ω Resistor (At least 1/4W recommended)
- 4x 1K Ω Resistors
- 2x 10K Ω Potentiometers
- 0.1µF Capacitor
- 4x LEDs

**Connect the following points together from this circuit and the one for ModbusRTUSlaveExample:**
- RS485_D-
- RS485_D+
- GND
