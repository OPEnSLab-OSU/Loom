# Project LOOM: Library

This is the primary location of the Project LOOM code, consolidated into place with a single Arduino .ino sketch built with code selected by the preprocessor based on the configuration file. 

## Table of Contents

1. [Device Support](#device-support)
    1. [Adafruit Feather M0](#adafruit-feather-m0)
    2. [Adafruit Feather 32u4](#adafruit-feather-32u4)
2. [Wireless Capabilities](#wireless-capabilities)
3. [Max/MSP](#max/msp)
4. [Library Architecture](#library-architecture)
    1. [Adding to the Library](#adding-to-the-library)
5.  [Channels](#channels)
    1.  [Implementation](#implementation)
    2. [Additional Devices](#additional-devices)
6. [Configuration File](#configuration-file)
7. [Arduino IDE Setup](#arduino-ide-setup)
8. [Glossary](#glossary)
9. [Troubleshooting](#troubleshooting)
10. [Other](#other)

## Device Support

- **Adafruit Feather M0**
- **Adafruit Feather 32u4**

## Communication Platforms

- **WiFi**
- **LoRa**

## Device Configuration

The configuration of the code to upload and flash to devices is set in the config.h file. This file presents the options that can be set or toggled to achieve a particular behavior. The preprocessor uses these definitions to select the necessary declarations, variables, and functions automatically. 

Currently changes to the config need to be done manually in the file itself. A script to automate this process from the command line, and ultimately Max, is in progress.

## Max/MSP

The associated [Loom Data Processors](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Max) and control modules are made as patches in Max/MSP.

## Librarary Architecture

The LOOM Library is the answer to the growing set of code to drive Project LOOM's supported devices, hardware, and communication platforms. The set of possible combinations of device configurations makes it unfeasable for LOOM developers and users alike to manage sketches with desired functionality. Instead, a configuration file effectively creates the desired sketch dynamically. A loom_preamble file selects entire files to include, each of which may also contain additional preprocessor statements.

### Overall Structure

The LOOM Library is effectively an aggregate of all of the functionality possible with the entirety of the supported devices, sensors, and actuators. The user then specifies the needs of their sketch inside the config.h. Based on the needs of the sketch, the requisite files, functions, and logic will be dynamically included such that only what is needed by the sketch is uploaded to the device. loom_preamble.h uses config.h to know which files to include, these files in turn then include the libraries they need. loom_common.h is also always present as it has the LOOM_begin() function, which sets up all the modules and sensors being used by the sketch. 

### Adding to the Library

New sensor and actuator support can be added by filling out the loom_mod_template.h file with relevant code. This template ensures that the code for each piece of hardware conforms to a standard format, making it easier to read and understand.

New files should be added #included in the loom_preamble.h file after config.h but generally before loom_flash.h and loom_common.h.

## Channels

The LOOM library and associated Max/MSP processors are designed to use channel based interfaces by default. Channel interfaces abstract device instance number, UDP port, and IP address into a single letter A, B, C, … H. 

The channels are purely a convenience, and the devices can operate without any notion of channels in the firmware and/or Max patches.

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

The Max interfaces presently support 8 concurrent channels, the library itself poses no such restrictions and using custom Max interfaces can allow for the control of more devices.

## Configuration File

The configuration file is used in conjunction with preprocessor statements to essentially built the specified sketch. Any options that can be set or toggled, or any specification of hardware (e.g. sensors and actuators) being used occur in (and only in) this config.h.

Any options for custom additions to the library should be added only to the configuratoin file.

## Configuration File Generation Script

Work is currently be done on implementing a script that will build the config.h file from the command line, taking the options as arguments. A corresponding Max interface will be made to provide a GUI to these options. The interface will pair with the one to subsequently build and upload code to devices.

## Arduino IDE Setup

## Building and Uploading Code Without The IDE

How to build and upload the code from the command line is currently being investigated and will eventually be implemented as runnable script. This script will then have a corresponding Max interface for uploading code to devices.

## Glossary

- **Patch:** a Max/MSP control interface or processor

## Troubleshooting

## Other
