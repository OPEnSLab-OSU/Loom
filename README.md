# Project Loom

## Table of Contents
1. [Processors](#processors)
    1. [Adafruit Feather M0](#adafruit-feather-m0)
    2. [Adafruit Feather 32u4](#adafruit-feather-32u4)
1. [Wireless Capabilities](#wireless-capabilities)
    1. [WiFi](#wifi)
    2. [LoRa](#lora)
        1. [LoRa Dependencies](#lora-dependencies)
        2. [EnableInterrupt.h and RH\_RF95.h compatibility](#enableinterrupth-and-rhrf95h-compatibility)
    3. [nRF](#nRF)
        1. [nRF Dependencies](#nrf-dependencies)
        2. [Configuring Maximum Message Length](#configuring-maximum-message-length)
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

Resources:
* [Adafruit M0 Documentation](https://learn.adafruit.com/adafruit-feather-m0-basic-proto/overview)
* [ATSAMD21 Datasheet](https://cdn-shop.adafruit.com/product-files/2772/atmel-42181-sam-d21_datasheet.pdf)

The compiler macro `__SAMD21G18A__` can be used to define code blocks specifically for the Feather M0.
For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_M0`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __SAMD21G18A__
#define is_M0
#endif
```

### Adafruit Feather 32u4

Resources:
* [Adafruit 32u4 Documentation](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto?view=all)
* [ATMega32U4 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

The compiler macro `__AVR_ATmega32U4__` can be used to define code blocks specifically for the Feather 32u4.
For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_32U4`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __AVR_ATmega32U4__
#define is_32U4
#endif
```

## Wireless Capabilities

### WiFi

### LoRa

LoRa wireless communication is supported by both the Adafruit Feather M0 and the
Adafruit Feather 32u4.

Features:
* Send messages up to 2 km line-of-sight.
* LoRa devices can be addressed with any value between 0 and 255.
* Can send messages of length up to 251 bytes.

#### LoRa Dependencies

All LoRa modules used by Project Loom are provided by the 
[RadioHead library](https://github.com/adafruit/RadioHead). This library
provides both the radio drivers (i.e. `RH_RF95.h`) and a network manager
(i.e. `RHReliableDatagram.h`).

#### EnableInterrupt.h and RH\_RF95.h compatibility

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

``` cpp
#define EI_NOTEXTERNAL
```

### nRF

The nRF flavor of IoA is comprised with the Nordic nRF24L01+ radio tranceiver.
nRF is supported by both the Adafruit Feather M0 and the Adafruit Feather 32u4.

Features:
* nRF devices can be addressed with any value between 0 and 7.
* Fragmentation allows for messages of any length to be sent with some configuration.
* Multi-hopping is supported.

#### nRF Dependencies

* [RF24](https://github.com/nRF24/RF24)
* [RF24Network](https://github.com/nRF24/RF24Network)

#### Configuring Maximum Message Length

The maximum message length can be adjusted by editing the value of `MAIN_BUFFER_SIZE`,
a variable found in the `RF24Network_config.h` file.

## Sensors

### I2C Sensors

### SDI-12 Sensors

The following SDI-12 sensors are currently supported by Project Loom:
* [Decagon GS3](http://manuals.decagon.com/Integration%20Guides/GS3%20Integrators%20Guide.pdf)
* [Decagon 5TM](http://www.ictinternational.com/content/uploads/2014/03/5TM-Integrators-Guide.pdf)

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

Open Sound Control (OSC) is the transmission protocol used by Project Loom.  The
Arduino OSC implementation, along with more information about OSC,
can be found [here](https://github.com/CNMAT/OSC).  

While OSC Bundles can be sent directly using WiFi, bundles must be reencoded to
transmit them via LoRa or nRF.  The OSC Interpreter allows OSC Bundles to be
translated into strings and allows strings to be translated back into OSC Bundles.
Each message address, along with corresponding data values, are concatenated into
a comma delimited string, and all messages in the bundle are concatenated into a
space delimited string.  Individual data values are encoded as 32-bit unsigned longs.
The following shows how the supported data values (i.e. float, int32\_t, and strings):


