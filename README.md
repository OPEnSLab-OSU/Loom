# Project Loom

## Table of Contents
1. [Processors](#processors)
    1. [Adafruit Feather M0](#adafruit-feather-m0)
    2. [Adafruit Feather 32u4](#adafruit-feather-32u4)
1. [Wireless Capabilities](#wireless-capabilities)
    1. [WiFi](#wifi)
    2. [LoRa](#lora)
    3. [nRF](#nRF)
2. [Sensors](#sensors)
    1. [I2C Sensors](#i2c-sensors)
    2. [SDI-12 Sensors](#sdi-12-sensors)
        1. [SDI-12 Dependencies](#sdi-12-dependencies)
        2. [Supported SDI-12 Pins](#supported-sdi-12-pins)
    3. [SPI Sensors](#spi-sensors)
3. [Actuators](#actuators)
4. [Miscellaneous Functionality](#miscellaneous-functionality)
    1. [RTC and Low Power Functionality](#rtc-and-low-power-functionality)
    2. [OSC Interpreter](#osc-interpreter)

## Processors

### Adafruit Feather M0

### Adafruit Feather 32u4

## Wireless Capabilities

### WiFi

### LoRa

### nRF

## Sensors

### I2C Sensors

### SDI-12 Sensors

#### SDI-12 Dependencies
* [Enable Interrupt](https://github.com/GreyGnome/EnableInterrupt)
* [SDI-12 Functionality](https://github.com/EnviroDIY/Arduino-SDI-12)

#### Supported SDI-12 Pins

The following pins are available for use with SDI-12 sensors:

* **Feather M0:** 10, 11, A0, A1, A3, A4, A5
* **Feather 32u4:** 10, 11

Additional pins may be operational, but they have not been verified.

**NOTE:** To use the SDI-12 library with the 32u4, the user must define their own
pin interrupts using the Enable Interrupt library.  The following line from
the file SDI12.h must be uncommented so that external interrupts can be defined:

``` cpp
#define SDI12_EXTERNAL_PCINT // uncomment to use your own PCINT ISRs
```

The following is an example of how the Enable Interrupt library can be used
to define an interrupt on pin 10:

``` cpp
pinMode(10, INPUT_PULLUP);
enableInterrupt(10, handler_function, CHANGE);
```

### SPI Sensors

## Actuators

## Miscellaneous Functionality

### RTC and Low Power Functionality

### OSC Interpreter
