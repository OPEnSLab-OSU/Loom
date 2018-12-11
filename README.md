# Project Loom: Library Overview

This is the primary location of the Project Loom code, consolidated into place with a single Arduino .ino sketch built with code selected by the preprocessor based on the configuration file. 

Other readmes are provided for more specific content, while this document focuses on providing an overview of what Loom is and how the library works.

#### Additional Readmes

- [Arduino and Loom Setup](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Arduino_and_Loom_Setup) – How to setup Arduino and Loom from scratch
- [Hardware](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/Readme_Hardware.md) – The processors, sensors, actuators, etc. that Loom uses / supports 
- [Using Loom](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Using_Loom.md) – How to use Loom once you have set it and the Arduino IDE up
- [Extending Loom](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Extending_Loom.md) – How to add support to the Loom Library
- [Hub Scripts](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Hub_Scripts.md) – Documentation on the mini programs / scripts that Loom supports
- [Loom Translator](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Loom_Translator.md) – Documentation on the collection of Loom functions for converting between data types and organization
- [Max MSP](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Max) – Details on using Max MSP data processors with WiFi enabled Loom devices
- [Limitations and Bugs](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/Readme_Limitations_and_Bugs.md) – The processors, sensors, actuators, etc. that Loom uses / supports 



## Table of Contents

1. [Installation](#installation)
2. [Device Support](#device-support)
3. [Communication Platforms](#communication-platforms)
4. [Device Configuration](#device-configuration)
5. [Max/MSP](#max/msp)
6. [Library Architecture](#library-architecture)

    1. [Overall Structure](#overall-structure)
        1. [Example Library Include Hierarchy](#example-library-include-hierarchy)
7. [Channels](#channels)
    1. [Implementation](#implementation)
    2. [Additional Devices](#additional-devices)
8. [Device Identification Hierarchy](#device-identification-hierarchy)
9. [SD Card Support](#sd-card-support)
10. [Real-Time Clock Support](#real-time-clock-support)
11. [Non-Volatile Flash and EEPROM memory](#non-volatile-memory)
12. [Configuration File](#configuration-file)
13. [API](#api)
14. [Using the Loom Library](#using-the-loom-library)



## Installation

See our [Arduino and Loom Setup Guide](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Arduino_and_Loom_Setup)



## Device Support

- Adafruit Feather M0
- Adafruit Feather 32u4 *(limited support)*



## Communication Platforms

WiFi and LoRa have had the most attention in development and thus have the most support and options, especially WiFi, as that is currently the only to communicate with a computer running Max/MSP. Other platforms have been implemented however. Below is the table of device inputs and outputs currently implemented, planned, or in consideration (bold for well implemented, * if partially implemented or in need of testing to be considered well implemented, plaintext if planned but not implemented, ? if in consideration). The top section is device-device communication platforms, and the bottom section are platforms that do not necessarily transmit directly to another Loom device

| Inputs         | Outputs                     | Comments                                                     |
| -------------- | --------------------------- | ------------------------------------------------------------ |
| **WiFi**       | **WiFi**                    | Implemented                                                  |
| **LoRa**       | **LoRa**                    | Implemented                                                  |
| **nRF**        | **nRF**                     | Implemented                                                  |
| Ethernet       | Ethernet*                   | Currently only used with hubs, expansion in progress         |
| GSM            | GSM*                        | Currently only one device, so outputs MQTT via GSM, but no GSM receiving yet |
|                |                             |                                                              |
| Google Sheets? | Google Sheets (PushingBox)* | Works as output, looking for ways of improving the pipline   |
| Adafruit IO*   | Adafruit IO*                | Needs proper integration                                     |
| IFTTT?         | IFTTT?                      | Currently, IFTTT works only as output though Adafruit IO     |
| SD card?       | **SD card**                 | Works as output, considering as input                        |
| Serial?        | **Serial**                  | Works for output. Serial as input may or may not be useful (is how Fona is tested) |
|                | **OLED**                    | Display a bundle on an Adafruit OLED featherwing / breakout  |



## Device Configuration

The configuration of the code to upload and flash to devices is specified in the config.h file. This file presents the options that can be set or toggled to achieve a particular behavior. The preprocessor uses these definitions to select the necessary declarations, variables, and functions automatically. 

Currently changes to the config need to be done manually in the file itself. An interface to aid the process is in progress.



## Max/MSP

The associated [Loom Data Processors](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Max) and control modules are made as patches in Max/MSP. The Max interfaces are not strictly necessary for using this library, but provide convenient features and means of interacting with your Loom devices / network.



## Librarary Architecture

The Loom Library is an answer to the growing set of code to drive Loom's supported devices, hardware, and communication platforms. The set of possible combinations of device configurations makes it unfeasable for Loom developers and users alike to manage sketches with desired functionality. Instead, a configuration file effectively creates the desired sketch dynamically. A loom_preamble.h file uses that configuration to select entire files to include, each of which may also contain additional preprocessor statements.

### Overall Structure

The Loom Library is effectively an aggregate of all of the functionality possible within the entirety of the supported devices, sensors, and actuators. The user then specifies the needs of their sketch inside the config.h. Based on the needs of the sketch, the requisite files, functions, and logic will be dynamically included such that only what is needed by the sketch is uploaded to the device. loom_preamble.h uses config.h to know which files to include, these files in turn then include the libraries they need. loom_common.h is also always present as it has the Loom_begin() function, which sets up all the modules and sensors being used by the sketch. 

#### Example Library Include Hierarchy

The hierarchy of included files looks something like the following (example if building for Ishield device on Feather M0 WiFi)  *(actual files may have since changed, but the idea/hierarchy remain)*. 

**Note** – bolded are files that are always necessary and thus included, independent of config.h. In angle brackets are non-Loom dependencies.

- Ishield_Example.ino
  - **Config.h**
  - **Loom_preamble.h** (likely to be renamed)
    - **<OSCBundle.h>**
    - Loom_translator.h
    - <Adafruit_SleepyDog.h>
    - <SPI.h>
    - loom_analog.h
    - loom_neopixel.h
      - <Adafruit_NeoPixel.h>
    - loom_mpu6050.h
      - <I2Cdev.h>
      - <Wire.h>
      - <MPU6050_6Axis_MotionApps20.h>
    - loom_wifi.h
      - <WiFi101.h>
      - <WiFiUDP.h>
    - **loom_flash.h**
      - <FlashStorage.h>
    - **loom_common.h**



### Message Router

#### Intro to Route and Dispatch of Message Router

The message router is called via:

```
bndl->route(configuration.packet_header_string, msg_router);
```

From the `process_bundle` function in the 'loom_interface.h' file. The message router checks any OSC messages that it receives, and if the message's address string matches one of the dispatch calls in the router, then the message gets forwarded to a function designed to handle / process messages with that address string. This usually entails reading the arguments to the message (which should be sent in an order / format known by the function that will handle the processing).

The dispatch calls in the message router take the following form:

```
msg.dispatch("/address", processing_function, addrOffset);
```

An example of a dispatch that takes a message that has an address of the device identifying string followed by "setID", and calls a function `set_instance_num` (which sets the device instance number) would be:

```
msg.dispatch("/SetID", set_instance_num, addrOffset);
```

The reason only the `/SetID` is provided and not something like `/LOOM/Ishield7/SetID` is because the aforementioned route function checks for message addressed to the device, which in this case is identified by `/LOOM/Ishield7`, and passes only the remaining part of the address to the message router. Messages that do not start with the device indentifying string are not sent to the message router.



## Channels

The LOOM library and associated Max/MSP processors are designed to use channel based interfaces by default. Channel interfaces abstract device instance number, UDP port, and IP address into a single letter A, B, C, … H. 

The channels are purely a user convenience, and the devices can operate without any notion of channels in the firmware and/or Max patches.

### Implementation
A common port is used for the communication of device settings in order to allow the Max patches to communicate with devices in a consistent manner if the instance number, UDP port, or IP (i.e. the channel information) address are unknown.

**Common Port:** 9440 

**Device Unique Channnel Data:** 9441 – 9448

Channel A:	Instance # = 1		UDP Port = 9441	

Channel B:	Instance # = 2		UDP Port = 9442

• • •

Channel H:	Instance # = 8		UDP Port = 9448

**IP Address:** Automatically assigned to devices by one's router. Max interfaces will automatically fetch IP address of selected device. If not using the channels feature implemented in the Loom Max interfaces, then Loom Debug mode may need to be enabled, or the user may need to add custom code, in order to obtain the device's IP address.

### Additional Devices

The Max interfaces presently support upto 8 concurrent channels, though the library itself poses no such restrictions and using custom Max interfaces can allow for the control of more devices.



## Device Identification Hierarchy

In Loom, a device is uniquely identified by 4 elements, from broad to precise identification:

- Family
- Family number
- Device type
- Device number

The OSC messages and bundles send to and from devices are address/signed with their ID string, in the following format:

`/<Family><Family-Num>/<Device-Type><Device-Num>/<the-rest-of-the-address>`

For example, Ishield 7 in the 3 'Loom' subnet would be:

`/Loom3/Ishield7`

**Family**

A family is generally a complete network of a given functionality under a single person or group.

**Family Number**

A family number is used to distinguish subnets between a family. This is typically used to seperate devices into groups by purpose, location, etc. In Max/MSP, you will generally only be observing a single subnet at a time. Family numbers should be 0 or greater.

**Device Type**

The Loom Library supports an autoname feature to try to name your device if it fits into a common hardware / functionality set. These include devices like Ishields, multiplexers, servo controllers, etc. and name the device accordingly. Alternatively, you can use custom names for your devices if you want. 

**Device Number / Channel**

A device number is used to distinguish the devices on the same subnet (family number) that have the same device type. Device numbers are sometimes also referred to as channels. Device numbers are expected to be between 1 and 8, inclusive (channels A-H).

### WiFi UDP Ports

If using WiFi, the family number and device number will determine the UDP ports that the device will expect messages from. The device will check and send on 3 ports, one for global messages, one for subnet, and one for device specific messages. The calculation of the specific ports are shown below. The reason for different ports fro various devices/purposes is to reduce the number of messages a device may recieve that were not intended for it.

### Device Targetting Scope

The message routing function of the Loom Library supports receiving messages addressed at a global, subnet, or device level. Max/MSP can send on any of the 3 formats as well.

**Global** 

Broadcast the OSC bundle to all devices. Those with the matching family name can see/react to the message. If using WiFi the message is sent on UDP port `9400`.

`/<Family>/<command>`

**Subnet**

Broadcast to a single subnet of a given family. Those with the matching family name and family number can see/react to the message. If using WiFi the message is sent on UDP port `9400+10*(family-num)`.

`/<Family><Family-Num>/<command>`

**Device**

Target a message at a single device, i.e. the one with the matching family, family number, device type, and device number. If using WiFi the message is sent on UDP port `9400+10*(family-num)+(device-num)`.

`/<Family><Family-Num>/<Device-Type><Device-Num>/<command`

### LoRa Subnet

Loom also supports subnets with LoRa. This is implemented by having the receiver check the source of the received OSC bundle (from the device identification address), and filter out unwanted bundles (i.e. empty them). 

The options are specified in the config.h file by `lora_subnet_scope`:

- 1: subnet
- 2: global/family
- 3: global only (not recommended)
- 4: any message. 

(use 2 if unsure)

These achieve a similar subnet structure that is produced with UDP ports on the WiFi boards.

### Diagram of Device Scope

![Loom_Hierarchy_Diagram](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/Images_for_Readmes/Loom_Hierarchy_Diagram.png)



## SD Card Support

The Loom library extends the Arduino SD library to have more features, mostly tailored towards using common data structures used by the library.

Namely, support for printing:

- Single elements (int, floats, c-strings, Strings)
- Arrays (ints, floats, c-strings, Strings)
- Bundles (single or multi-msg format)

Additionally, if used with MaxMSP, there is support for sending commands to enable / disable the logging of bundles the device receives (also works without Max).



## Real-Time Clock Support

The Loom library supports two RTC devices:

- [Adafruit DS3231](https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/)
- [Adafruit PCF8523](https://learn.adafruit.com/adafruit-adalogger-featherwing)

Currently, interrupts (for waking the microprocessor from sleep) are only supported via the DS3231, with the PCF8523 mostly being used in conjunction with an SD card (as Adafruit Adalogger featherwing has PCF8523 and SD card holder) to provide timestamps.

The Loom Library has support for both devices for returning strings of the date, time, or weekday. Additional timer functions are provided for the DS3231.

#### RTC and Low Power Dependencies

- [DS3231 Extended Library](https://github.com/FabioCuomo/FabioCuomo-DS3231)
- [Low Power Library](https://github.com/rocketscream/Low-Power)
- [Enable Interrupt](https://github.com/GreyGnome/EnableInterrupt)

**NOTE:** To use the DS3231 extended library with the Feather M0,
the following line must be added to `RTClibExtended.h`:

```cpp
#define _BV(bit) (1 << (bit))
```

#### Sleep Modes

Project Loom supports two sleep modes for the Feather M0 and one sleep mode for the Feather 32u4.
Here are some details on the various modes:

| Mode           | Supported board | Current Draw |
| -------------- | --------------- | ------------ |
| Idle\_2        | Feather M0      | ~5 mA        |
| Standby        | Feather M0      | ~0.7 mA      |
| SLEEP\_FOREVER | Feather 32U4    | Untested     |

#### Standby Operation

Due to some incompatibilities between Standby mode and falling interrupts, a very particular
scheme must be followed to use Standby mode on the Feather M0.  The following code is an
example of how standby mode can be set up on the M0 with a wakeup interrupt on pin 11:

``` cpp
void setup() {
    pinMode(11, INPUT_PULLUP);
    bool OperationFlag = false;
    delay(10000); //It's important to leave a delay so the board can more easily
                  //be reprogrammed
}

void loop() {
    if (OperationFlag) {

        // Whatever you want the board to do while awake goes here

        OperationFlag = false; //reset the flag
    }

    attachInterrupt(digitalPinToInterrupt(11), wake, LOW);

    LowPower.standby();
}

void wake() {
    OperationFlag = true;
    detachInterrupt(digitalPinToInterrupt(11)); //detach the interrupt in the ISR so that
                                                //multiple ISRs are not called
}
```



## Non-Volatile Flash and EEPROM memory

For storing configurations, both the Feather M0 and the Feather 32u4 boards are capable of storing a
struct in non-volatile memory. The functions related to non-volatile memory can be found in loom_flash.h,
and are automatically compatible with your board. If you modify the contents of the configuration structure,
you can save them with write_non_volatile(). If you need to read from flash memory, you can read into the
configuration structure with read_non_volatile(). Do not excessively write, as each board supports about
~10000 flash writesin its lifetime; the cells in flash memory wear out over time.



## Configuration File

The configuration file is used in conjunction with preprocessor statements to build the specified sketch, with as small a program memory usage as possible. Any options that can be set or toggled, or any specification of hardware (e.g. sensors and actuators) being used occur in this config.h.

Any options for custom additions to the library should be added only to the configuration file.

The config.h file needs to be included before the library (via loom_preamble.h) itself, so that the configuration can be used to define the necessary subset of the library.



## API

*'[]'s are used to indicate arguments that are optional or specific to a given platform* 



- Receive Bundle `void receive_bundle(OSCBundle *bndl, CommPlatform platform)`
  - Receive a packet and put it in an OSC bundle if not already
  - CommPlatform options:
    - WIFI
    - LORA
    - NRF
- Process Bundle `void process_bundle(OSCBundle *bndl)`
  - Process a bundle (presumably recieved via recieve_bundle) by performing message routing on it 
- Measure Sensors `void measure_sensors()`
  - Measure any enabled sensors
- Package Data `void package_data(OSCBundle *send_bndl)`
  - Package measured data into an OSC bundle  
- Send Bundle `void send_bundle(OSCBundle *send_bndl, CommPlatform platform)` 
  - Send a bundle (presumably built via package_data) to another Loom device / MaxMSP
- Log Bundle `void log_bundle(OSCBundle *send_bndl, LogPlatform platform [,char* file])`
  - Export a bundle
  - Platform options
    - PUSHINGBOX (Google Sheets)
    - SDCARD (requires filename)
    - OLED
- Additional Loop Checks `void additional_loop_checks()`
  - Function to catch remaining functionality that is not contained by the rest of the API

Further details on using the API can be found [here](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Using_Loom.md)



## Using the Loom Library

Details on setting up Arduino IDE and Loom can be found [here](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Arduino_and_Loom_Setup)

Details on using Loom once setup (with examples) can be found [here](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/ReadMe_Using_Loom.md)

#### 