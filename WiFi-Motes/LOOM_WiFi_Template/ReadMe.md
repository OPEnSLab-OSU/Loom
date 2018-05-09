# Project LOOM: Library

This is the primary location of the Project LOOM code, consolidated into place with a single Arduino .ino sketch built with code selected by the preprocessor based on the configuration file. 

## Max/MSP

The associated Loom data processors and control modules are made as patches in Max/MSP

## Librarary Architecture

The LOOM Library is the answer to the growing set of code to drive Project LOOM's supported devices, hardware, and communication platforms. The set of possible combinations of device configurations makes it unfeasable for LOOM developers and users alike to manage sketches with desired functionality. Instead, a configuration file effectively creates the desired sketch dynamically.

### Adding to the Library

New sensors and actuator support can be added by filling out the loom_mod_template.h file with relevant code. This template ensures that the code for each piece of hardware conforms to a standard format, making it easier to read and understand.

## Device Support

**Adafruit Feather M0**

**Adafruit Feather 32u4**

## Device Configuration

The configuration of the code to upload and flash to devices is set in the config.h file. This file presents the options that can be set or toggled to achieve a particular behavior. The preprocessor uses these definitions to select the necessary declarations, variables, and functions automatically. 

Currently changes to the config need to be done manually in the file itself. A script to automate this process from the command line, and ultimately Max, is in progress.

## Channels

The LOOM library and associated Max/MSP processors are designed to use channel based interfaces by default. Channel interfaces abstract device instance number, UDP port, and IP address into a single letter A, B, C ... H. 

### Implementation
A common port is used for the communication of device settings, in order to allow the Max patches to communicate with devices in a consistent manner if the instance number, UDP port, or IP address are unknown.

**Common Port:** 

9440 

**Device Unique Channnel Data:**

Channel A:	Instance # = 1		UDP Port = 9441	
Channel B:	Instance # = 2		UDP Port = 9442
• • •

**IP Address:** 

Automatically assigned to devices by router. Max interfaces will automatically fetch IP address of selected device.

### Additional Devices

The Max interfaces presently support 8 concurrent channels, the library itself poses no restrictions and using the Expert Loom Max interfaces can allow control of more devices

## Troubleshooting

## Other