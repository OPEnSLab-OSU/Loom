# Project Loom: Library

This is the primary location of the Project LOOM code, consolidated into place with a single Arduino .ino sketch built with code selected by the preprocessor based on the configuration file. 

## Table of Contents

1. [Installation](#installation)

2. [Device Support](#device-support)

3. [Communication Platforms](#communication-platforms)

4. [Device Configuration](#device-configuration)

5. [Max/MSP](#max/msp)

6. [Library Architecture](#library-architecture)
    1. [Overall Structure](#overall-structure)
        1. [Example Library Include Hierarchy](#example-library-include-hierarchy)
    2. [Adding to the Library](#adding-to-the-library)
    3. [Adding to the Message Router](adding-to-the-message-router)

7. [Channels](#channels)
    1. [Implementation](#implementation)
    2. [Additional Devices](#additional-devices)

8. [Configuration File](#configuration-file)

9. [API](#api)
    1. [Includes](#includes)
    2. [Setup](#setup)
    3. [Main Loop Functions](#main-loop-functions)
        1. [Receive Bundle](#receive-bundle)
        2. [Process Bundle](#process-bundle)
        3. [Measure Sensors](#measure-sensors)
        4. [Package Data](#package-data)
        5. [Send Bundle](#send-bundle)
        6. [Additional Loop Checks](#additional-loop-checks)
        	. [Minimal Working Example](#minimal-working-example)	

10. [SD Card Support](#sd-card-support)

11. [Real-Time Clock Support](#real-time-clock-support)

12. [Arduino IDE Setup](#arduino-ide-setup)

13. [Building and Uploading Code Without The IDE](#building-and-uploading-code-without-the-ide)

14. [Configuration Conflicts](#configuration-conflicts)

   ​      

## Installation

The Loom Library, and any dependencies, need to be placed into the Arduino libraries folder, i.e:

Document > Arduino > Libraries

## Device Support

- Adafruit Feather M0
- Adafruit Feather 32u4

## Communication Platforms

WiFi and LoRa have had the most attention in development and thus have the most support and options, especially WiFi, as that is currently the only to communicate with a computer running Max/MSP. Other platforms have been implemented however. Below is the table of device inputs and outputs currently implemented, planned, or in consideration (bold for well implemented, * if partially implemented or in need of testing to be considered well implemented, plaintext if planned but not implemented, ? if in consideration). The top section is device-device communication platforms, and the bottom section are platforms that do not necessarily transmit directly to another Loom device

| Inputs         | Outputs                     | Comments                                                     |
| -------------- | --------------------------- | ------------------------------------------------------------ |
| **WiFi**       | **WiFi**                    | Implemented                                                  |
| **LoRa**       | **LoRa**                    | Implemented                                                  |
| nRF*           | nRF*                        | May need more testing to be 'proven'                         |
| Ethernet       | Ethernet*                   | Currently only used with hubs, expansion in progress         |
| GSM            | GSM*                        | Currently only one device, so outputs MQTT via GSM, but no GSM receiving yet |
| Bluetooth      | Bluetooth                   | Planned                                                      |
|                |                             |                                                              |
| Google Sheets? | Google Sheets (PushingBox)* | Works as output, looking for ways of improving the pipline   |
| Adafruit IO*   | Adafruit IO*                | Could use more robust integration                            |
| IFTTT?         | IFTTT?                      | Currently, IFTTT works only as output though Adafruit IO     |
| SD card?       | **SD card**                 | Works as output, considering as input                        |
| Serial?        | **Serial**                  | Works for output. Serial as input may or may not be useful (is how Fona is tested) |

## Device Configuration

The configuration of the code to upload and flash to devices is set in the config.h file. This file presents the options that can be set or toggled to achieve a particular behavior. The preprocessor uses these definitions to select the necessary declarations, variables, and functions automatically. 

Currently changes to the config need to be done manually in the file itself. A script to automate this process from the command line, and ultimately Max, is in progress.

## Max/MSP

The associated [Loom Data Processors](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Max) and control modules are made as patches in Max/MSP. The Max interfaces are not strictly necessary for using this library, but provide convenient features and means of interacting with your Loom devices / network.

## Librarary Architecture

The LOOM Library is the answer to the growing set of code to drive Project LOOM's supported devices, hardware, and communication platforms. The set of possible combinations of device configurations makes it unfeasable for LOOM developers and users alike to manage sketches with desired functionality. Instead, a configuration file effectively creates the desired sketch dynamically. A loom_preamble file selects entire files to include, each of which may also contain additional preprocessor statements.

### Overall Structure

The LOOM Library is effectively an aggregate of all of the functionality possible within the entirety of the supported devices, sensors, and actuators. The user then specifies the needs of their sketch inside the config.h. Based on the needs of the sketch, the requisite files, functions, and logic will be dynamically included such that only what is needed by the sketch is uploaded to the device. loom_preamble.h uses config.h to know which files to include, these files in turn then include the libraries they need. loom_common.h is also always present as it has the LOOM_begin() function, which sets up all the modules and sensors being used by the sketch. 

#### Example Library Include Hierarchy

The hierarchy of included files looks something like the following (example if building for Ishield device on Feather M0 WiFi). 

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

### Adding to the Library

New sensor and actuator support can be added by filling out the loom_mod_template.h file with relevant code. This template ensures that the code for each piece of hardware conforms to a standard format, making it easier to read and understand.

New files should be added #included in the loom_preamble.h file after config.h but generally before loom_flash.h and loom_common.h.

There are plans/consideration for a script that will handle most of the additions for a user based on various provided parameters / needs as current additions generally need to modify the config, preamble, loom_commom (sometimes), and loom_interface files in a mostly boiler plate fashion in addition to the new device-specific file.

### Message Router

#### Intro to Route and Dispatch of Message Router

Custom functions can be added to the message router. The message router is called via:

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

#### Adding Custom Functions to Message Router

There are a few steps that must be followed in order to get a custom function to be callable by the message router. These are:

- Write your function in the correct format
- Add the function prototype to the top of loom_msg_router.h file
- Add the dispatch call to the msg_router function

**Writing Your Function**

All function that are called by the message router need to take only an OSC message by reference, and return nothing. Thus a function skeleton would look like:

```
void your_function(OSCMessage &msg)
{
	// process the message forwarded from msg_router
}
```

Inside this function is generally where you will process the message (if you need, you can add auxiliary function(s), to do some of the processing/handling - these do not have constraints on the parameters or return value). The following OSC methods are recommended for extracting the arguments from the message. (The below list is not exhaustive, see the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md) for more functions)

**`msg.size()`** Returns the number of arguments in the the message

**`msg.getType(index)`** Returns the type of the argument at `index`, `i` for integer, `f` for float, and `s` for string. Useful in combination with switch statement to select which of the following three functions to call.

**`msg.getInt(index)`** Returns argument to message that is an integer at `index` if argument is indeed an integer and there is an argument at that index

**`msg.getFloat(index)`**Returns argument to message that is a float at `index` if argument is indeed a float and there is an argument at that index

**`msg.getString(index)`**Returns argument to message that is a string at `index` if argument is indeed a string and there is an argument at that index

**`msg.getAddress(buf, 0);`** Copies address of message into char array called `buf`

**Adding the Function Prototype**

Add the protype of the function you wrote, e.g:

```
void your_function(OSCMessage &msg);
```

to the `Custom Function Prototypes` section at the top of the `loom_msg_router.h` file. (Don't forget the semicolon). Should be no need to add the prototypes for any auxililary functions called by the `your_function`.

**Adding the Dispatch Call**

At the bottom of the message router, add:

```
msg.dispatch("/command/string", your_function, addrOffset);
```

Where

-  `/commmand/string` is the section after the device identifying string that is being used to select which function to dispatch a message to. This should be descriptive but not overly long. In the above example,  the string `setID` was used to check if the message should be forwarded to `set_instance_num`
- `your_function` is the name of the function you want to be called if the message matches the device identifying string and command string.
- `addrOffset` should stay as it is, this is an integer value passed in from the `bndl->route(...)` call, and simply specifies where in the message string to start looking for the command section (i.e. right after the device identifying string)

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

Any options for custom additions to the library should be added only to the configuration file.

The config.h file needs to be included before the library itself, so that the configuration can be used to define the necessary subset of the library.

## API

### Includes

Only the configuration and the library are necessary in the .ino sketch. All necessary dependecies of the Loom Library are automatically included. 

**Note:** The config.h file must precede the library

```cpp
#include "config.h"
#include "loom_preamble.h"
```

### Setup

All Loom and device setup is called dynamically within Loom_begin(). A simple setup function would simply be:

```cpp
void setup() 
{
	Loom_begin();	
    
    // Any custom setup code
}
```

### Main Loop Functions

The Loom Library has six primary interface function to call. With just these, a fully functional sketch can be made. Custom code can be inserted before, between, and after any of the interface functions, but changing the relative ordering of the interface functions is not recommended unless you know what you are doing.

#### Receive Bundle

Fills an OSC bundle with packets received the specified platform if data exists and platform is enabled.

```cpp
// Receive bundles
//  takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
receive_bundle(&bndl, WIFI);
```

- **bndl** – The bundle to fill
- **platform** – The wireless platform to receive on, the values are encoded to a Platform enum to reduce chance for errors

#### Process Bundle

Examine the provided OSC bundle (presumably filled via receive_bundle(). If bundle is not empty,  has no errors, and is addressed to this device, then attempt to perform action specified.

```cpp
// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
process_bundle(&bndl);
```

- **bndl** – The bundle to be processed

#### Measure Sensors

Update stored readings from sensors by calling measure on each enabled sensor.

```cpp
// Update stored readings from sensors
measure_sensors();
```

#### Package Data

Fill the provided OSC bundle with latest stored sensor readings

```cpp
// Populate bundle to send with sensor values
package_data(&send_bndl);
```

- **send_bndl** – The OSC bundle to be filled

#### Send Bundle

Sends a packaged bundle on the specified platform

```cpp
// Send the bundle
//  takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
send_bundle(&send_bndl, WIFI);
```

- **bndl** – The bundle to be sent
- **platform** – The wireless platform to send on, the values are encoded to a Platform enum to reduce chance for errors

#### Additional Loop Checks

Performs any miscellaneous Loom tasks that happen each loop iteration, but are not handled by any of the other 5 interface functions, such as delays between loop iterations and checking a device button has been held for a set amount of time.

```cpp
// Loop checks and sleep between iterations if enabled
additional_loop_checks();
```

### Minimal Working Example

This example is fully functional. It assumes that WiFi has been specified as a wireless communication platform in the config.h file.

```cpp
#include "config.h"
#include "loom_preamble.h"

void setup() 
{
	Loom_begin(); // LOOM_begin calls any relevant LOOM device setup functions
	// Any custom setup code
}

void loop() 
{
	OSCBundle bndl, send_bndl; 
	
	receive_bundle(&bndl, WIFI);
	process_bundle(&bndl);
	measure_sensors();
	package_data(&send_bndl);
	send_bundle(&send_bndl, WIFI);
	additional_loop_checks();
} 
```

## SD Card Support

The Loom library extends the Arduino SD library to have more features, mostly tailored towards using common data structures used by the library.

Namely, support for printing:

- Single elements (int, floats, c-strings, Strings)
- Arrays (ints, floats, c-strings, Strings)
- Bundles (single or multi-msg format)

Additionally, if used with MaxMSP, there is support for sending commands to enable / disable the logging of bundles the device receives (also works without Max).

## Real-Time Clock Support

The Loom library supports two RTC devices:

- Adafruit DS3231 
- Adafruit PCF8523

Currently, interrupts are only supported via the DS3231, with the PCF8523 mostly being used in conjunction with an SD card (as Adafruit Adalogger featherwing has PCF8523 and SD card holder) to provide timestamps.

The Loom Library has support for both devices for returning strings of the date, time, or weekday. Additional timer functions are provided for the DS3231.

## Arduino IDE Setup

Make sure to download the dependencies of the Loom Library from:

https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Dependencies 

and install the libraries with the Arduino IDE, as described here:

https://www.arduino.cc/en/guide/libraries

## Building and Uploading Code Without The IDE

Work is currently be done on investigating and implementing a script that will build the config.h file from the command line, taking the options as arguments. A corresponding Max interface will be made to provide a GUI to these options. The interface will pair with the one to subsequently build and upload code to devices.

The design is planned to be as follows:

- Generate the config file from command line 
  - Arguments to used set options, have default values if none specified
  - Sent to a Python script, inserting the parameters in the relevant locations of the config file
- Be able to call this script from Max
- Use Max to create a graphical interface of toggles / inputs corresponding to the script arguments.
  - Error checking (i.e., of selection conflicts) can be performed in the script or Max, but would doing so in Max would give users immediate feedback on invalid combinations


## Configuration Conflicts

The following combinations of options in the configuration file result in various conflicts (e.g. of dependencies) that are known to result in errors or undefined behavior. Such conflicts are also mentioned in the config.h file itself.

- LoRa + nRF
- is_sleep_period + LoRa (Adafruit_ASFcore-master/tc_interrupt.c:140: multiple definition of 'TC3_Handler')