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
        1. [Adafruit Universal Thermocouple Amplifier](#adafruit-universal-thermocouple-amplifier)
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

#### Adafruit Universal Thermocouple Amplifier

Project Loom currently supports operation of the Adafruit MAX31856 Universal
Thermocouple Amplifier in both K-type thermocouple operation and generic voltage
operation.  K-type thermocouple operation allows the user to directly measure
temperatures, when the amplifier is used in conjunction with a K-type thermocouple,
and the generic voltage operation allows the user to measure the voltage across any
thermocouple attached to the amplifier.

Dependencies and Documentation:
* [Adafruit MAX31856 Github](https://github.com/adafruit/Adafruit_MAX31856)
* [Adafruit MAX31856 Overview](https://learn.adafruit.com/adafruit-max31856-thermocouple-amplifier/overview)

**IMPORTANT NOTE:** The Adafruit thermocouple library has been modified to include functionality
to read the thermocouple voltage directly.  The modified library can be found
[here](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/Dependencies/Adafruit_MAX31856.zip).

Alternatively, the following function can be added as a public member to the Adafruit\_MAX31856
class in the Adafruit\_MAX31856 library:

``` cpp
float Adafruit_MAX31856::readVoltage(int gain) {
  oneShotTemperature();

  int32_t temp24 = readRegister24(MAX31856_LTCBH_REG);
  if (temp24 & 0x800000) {
    temp24 |= 0xFF000000;
  }

  temp24 >>= 5;

  float tempfloat = temp24/((float)(gain * 209715.2)); //temp24 = gain * 1.6 * 2^17 * vin
  
  return tempfloat;
}

```

## Actuators

## Miscellaneous Functionality

### RTC and Low Power Functionality

### OSC Interpreter
