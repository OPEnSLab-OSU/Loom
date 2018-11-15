# Project Loom: Using Loom



## Configuration File

The configuration file is used in conjunction with preprocessor statements to build the specified sketch, with as small a program memory usage as possible. Any options that can be set or toggled, or any specification of hardware (e.g. sensors and actuators) being used occur in this config.h.

Any options for custom additions to the library should be added only to the configuration file.

The config.h file needs to be included before the library (via loom_preamble.h) itself, so that the configuration can be used to define the necessary subset of the library.



## .ino File

### Overview

By default, the .ino sketch is fairly sparse – the config.h and Loom preamble are #included (which together define what parts of the library should be used). The `setup()` function simply calls `Loom_begin()`  to handle any of the setup of the Loom components. The `loop()` function declares an OSC Bundle or two and uses the API functions.

- Any global variables you need that are not already availalbe in Loom can be added above the `setup()` function
- Add any additional setup you need after the call to `Loom_begin()` in `setup()`
- Use the API to interface with the supported components of Loom at a high level
- Put your own code into the `loop()` function as needed to support your additional functionality / components

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

### Main Loop Functions / API

The Loom Library has six primary interface function to call. With just these, a fully functional sketch can be made. Custom code can be inserted before, between, and after any of the interface functions, but changing the relative ordering of the interface functions is not generally recommended unless you know what you are doing.

#### Receive Bundle

 `void receive_bundle(OSCBundle *bndl, CommPlatform platform)`

Fills an OSC bundle with packets received the specified platform if data exists and platform is enabled.

```cpp
// Receive bundles
//  takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
receive_bundle(&bndl, WIFI);
```

- **bndl** – The bundle to fill
- **platform** – The wireless platform to receive on, the values are encoded to a Platform enum to reduce chance for errors

#### Process Bundle

`void process_bundle(OSCBundle *bndl)`

Examine the provided OSC bundle (presumably filled via receive_bundle(). If bundle is not empty,  has no errors, and is addressed to this device, then attempt to perform action specified.

```cpp
// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
process_bundle(&bndl);
```

- **bndl** – The bundle to be processed

#### Measure Sensors

 `void measure_sensors()`

Update stored readings from sensors by calling measure on each enabled sensor.

```cpp
// Update stored readings from sensors
measure_sensors();
```

#### Package Data

`void package_data(OSCBundle *send_bndl)`

Fill the provided OSC bundle with latest stored sensor readings

```cpp
// Populate bundle to send with sensor values
package_data(&send_bndl);
```

- **send_bndl** – The OSC bundle to be filled

#### Send Bundle

`void send_bundle(OSCBundle *send_bndl, CommPlatform platform)`

Sends a packaged bundle on the specified platform

```cpp
// Send the bundle
//  takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
// ex:
send_bundle(&send_bndl, WIFI);
```

- **send_bndl** – The bundle to be sent
- **comm_platform** – The wireless platform to send to another device on, the values are encoded to a CommPlatform enum to reduce chance for errors
  - WIFI
  - LORA
  - NRF

#### Log Bundle

`void log_bundle(OSCBundle *send_bndl, LogPlatform platform [,char* file])`

Logs a packaged bundle on the specified platform. *(Is distinct from send_bundle in that the destination is not another Loom enabled microprocessor)*

```cpp
// Send the bundle
//  takes bundle to be filled and wireless platforms [PUSHINGBOX, SDCARD, OLED]
// ex:
log_bundle(&log_bndl, PUSHINGBOX);
// SD card ex:
log_bundle(&log_bndl, SDCARD, "filename.csv");

```

- **log_bndl** – The bundle to be logged
- **log_platform** – The platform to log to, the values are encoded to a LogPlatform enum to reduce chance for errors
  - PUSHINGBOX (Google Sheets)
  - SDCARD (requires filename)
  - OLED

#### Additional Loop Checks

Performs any miscellaneous Loom tasks that happen each loop iteration, but are not handled by any of the other 5 interface functions, such as delays between loop iterations and checking a device button has been held for a set amount of time.

```cpp
// Loop checks and sleep between iterations if enabled
additional_loop_checks();
```

### Directly Accessing Variables

While you would normally let the main API functions collect, manage, and forward the device's data, you can access the data points individually and directly. This includes sensor readings and global variables, and depending on which type, there may be multiple methods of getting to the data.

#### Extracting from OSC Bundle

If your data point is a sensor reading, the bundle populated by `package_data()` will contain the data point within. The bundles are structured in key-value pair messages, that is, each data point is contained within a single message, the key being the last part of the message address, and the data value being the single element in the message.

Using functions provided in the Loom Translator (documentation: [Loom Translator Readme](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/LOOM_library/ReadMe_Loom_Translator.md), file: [loom_translator.h](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/LOOM_library/loom_translator.h)) you can access the data elements of a bundle or convert the bundle to different formats, such as an array.

While the translator should be sufficient for most data extraction and conversions, you can also refer to the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md) for the specific functions to access/modify bundles (which some of the Translator functions are just wrappers of).

#### Accessing Global Structs

Most components in Loom (sensor, actuator, communication platform, etc...) have a state structure (C struct) that contains the readings and state of the component. In the case of sensors, the variables pertaining to the readings will be updated anytime the API function `measure_sensors()` is called. The states of other components will change in more variable patterns.

The states are defined near the top of the file of the component they are associated with, under a heading of "Structures." 

##### Reading

In the .ino file, you simply have to access the members of a given struct (location specified above) similar to:

```
int analog_val0 = state_analog.a0;
LOOM_DEBUG_println("Analog 0: ", analog_val0);  // print the analog value
```

**Writing**

While there is nothing currently preventing you from writing to the state structures, it is not recommended and would have undefined effects. However, you are free to experiment should you choose to.

#### Accessing Other Global Variables

The [loom_preamble.h](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/LOOM_library/loom_preamble.h) file contains a few other global variables that are general and not associated with a given component or struct. If you want to access them simply access them as normal global variables, like:

```
float batt_val = vbat; // access the battery value
```

#### Accessing Component Classes

Many of the Loom components have a class which Loom simply provides a wrapper for. The classes can be accessed as well, using whatever public functions it provides. The behavior of directly accessing these classes without or in addition to letting Loom handle them is undefined. This does not mean you cannot do this, just that further documentation on the process will not be provided.

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

    // Incoming bundles
    receive_bundle(&bndl, WIFI);
    process_bundle(&bndl);

    // Sensors
    measure_sensors();
    package_data(&send_bndl);

    // Outgoing bundles
    send_bundle(&send_bndl, WIFI);
    log_bundle(&send_bndl, PUSHINGBOX);

    int analog_val0 = state_analog.a0;    // directly access analog state struct
    LOOM_DEBUG_println("Analog 0: ", analog_val0);  

    additional_loop_checks();
} 
```

### **Modifying the .ino functionality**

It is recommeded that you base your code off the example provided / use the High-Level API Functions listed at the bottom of the .ino file to for more basic Loom setups. The `loom_interface.h` file describes each of those functions in further detail. 



## More Example .ino Loop Contents

This section provides some common examples of how you might format the `loop()` function in the .ino file.

#### WiFi Send and Receive (Ideal for use with MaxMSP)

```
void loop() 
{
	OSCBundle bndl, send_bndl;  		// Bundles to hold incoming and outgoing data

	receive_bundle(&bndl, WIFI);		// Receive messages
	if (bndl.size()) {
		print_bundle(&bndl);			// Print bundle if LOOM_DEBUG enabled
	}
	process_bundle(&bndl);				// Dispatch message to correct handling functions
	measure_sensors();					// Read sensors, store data in sensor structs
	package_data(&send_bndl);			// Copy sensor data from state to provided bundle
	send_bundle(&send_bndl, WIFI);		// Send bundle of packaged data
	additional_loop_checks();			// Miscellaneous checks
}
```

#### LoRa Hub (Recieve from node, upload to Google Sheet, no sensors)

```
void loop() 
{
	OSCBundle bndl;  					// Bundles to hold incoming and outgoing data

	receive_bundle(&bndl, LORA);		// Receive messages	
	log_bundle(&bndl, PUSHINGBOX);		// Send bundle of packaged data
	additional_loop_checks();			// Miscellaneous checks
}
```

#### LoRa Sensor Node (Transmits to central hub for upload to Google Sheet)

```
void loop() 
{
	OSCBundle send_bndl;  				// Bundles to hold incoming and outgoing data

	measure_sensors();					// Read sensors, store data in sensor structs
	package_data(&send_bndl);			// Copy sensor data from state to provided bundle
	send_bundle(&send_bndl, LORA);		// Send bundle of packaged data
	log_bundle(&send_bndl, SDCARD, "filename.csv"); // Save to SD card
	additional_loop_checks();			// Miscellaneous checks
}
```



## Using Sensors on Top of  Loom

This section explains how you might go about using non-Loom sensors with the rest of Loom functionality.

If you are not integrating you sensor into Loom, keep your code in the .ino file or any additional files you have made. 

#### **Components**

Components your code will likely have are:

- Include sensor library
- Possible instance of sensor class
- Setup
- Measure sensor

All of this can be done as normal, as if you were not using Loom. To get the data of your sensor to work with the rest of Loom, you will need to add the data to an OSC bundle.

#### Adding to Bundle

The package_bundle() API function populates the provided bundle. To add your own data to this bundle you can use the provided loom_translator functions, namely `append_bundle`. This function is overloaded and uses templates, so it can take most data types in either singular or array formats. You can also use any of the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md).

Most of the time Loom OSC bundles are in key-value pairs, that is, 1 message per data point, with the one element being the data point.

## Configuration Conflicts

The following combinations of options in the configuration file result in various conflicts (e.g. of dependencies) that are known to result in errors or undefined behavior. Such conflicts are also mentioned in the config.h file itself.

- LoRa + nRF
- is_sleep_period + LoRa (Adafruit_ASFcore-master/tc_interrupt.c:140: multiple definition of 'TC3_Handler')