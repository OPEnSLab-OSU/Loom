# Project Loom: Using Loom

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
// ex:
send_bundle(&send_bndl, WIFI);
```

- **send_bndl** – The bundle to be sent
- **comm_platform** – The wireless platform to send to another device on, the values are encoded to a CommPlatform enum to reduce chance for errors
  - WIFI
  - LORA
  - NRF

#### Log Bundle

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
  - PUSHINGBOX
  - SDCARD
  - OLED

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
	
    // Incoming
	receive_bundle(&bndl, WIFI);
	process_bundle(&bndl);
    
    // Sensors
	measure_sensors();
	package_data(&send_bndl);
    
    // Outgoing
	send_bundle(&send_bndl, WIFI);
    log_bundle(&send_bndl, PUSHINGBOX);
    
	additional_loop_checks();
} 
```

## Configuration Conflicts

The following combinations of options in the configuration file result in various conflicts (e.g. of dependencies) that are known to result in errors or undefined behavior. Such conflicts are also mentioned in the config.h file itself.

- LoRa + nRF
- is_sleep_period + LoRa (Adafruit_ASFcore-master/tc_interrupt.c:140: multiple definition of 'TC3_Handler')