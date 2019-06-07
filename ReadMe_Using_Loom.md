# Project Loom: Using Loom

## *To be updated*



## Configuration File



## .ino File

### Overview

By default, the .ino sketch is fairly sparse – the configuration defines what parts of the library should be used. The `setup()` function simply calls `Loom_begin()`  to handle any of the setup of the Loom components. The `loop()` function declares an OSC Bundle or two and uses the API functions.

- Any global variables you need that are not already available in Loom can be added above the `setup()` function
- Add any additional setup you need after the call to `Loom_begin()` in `setup()`
- Use the API to interface with the components of Loom at a high level
- Put your own code into the `loop()` function as needed to support your additional functionality / components

### Includes

Only the configuration and the library are necessary in the .ino sketch. All dependecies of Loom are automatically included. 

```cpp
#include <Loom.h>
#include "config.json"
```



### Setup



### Main Loop Functions / API



### Directly Accessing Variables / Measurements

While you would normally let the main API functions collect, manage, and forward the device's data, you can access the data points individually and directly. This includes sensor readings and global variables, and depending on which type, there may be multiple methods of getting to the data.

#### Extracting from OSC Bundle

If your data point is a sensor reading, the bundle populated by `package_data()` will contain the data point within. The bundles are structured in key-value pair messages, that is, each data point is contained within a single message, the key being the last part of the message address, and the data value being the single element in the message.

Using functions provided in the Loom Translator (documentation: [Loom Translator Readme](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/LOOM_library/ReadMe_Loom_Translator.md), file: [loom_translator.h](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/LOOM_library/loom_translator.h)) you can access the data elements of a bundle or convert the bundle to different formats, such as an array.

While the translator should be sufficient for most data extraction and conversions, you can also refer to the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md) for the specific functions to access/modify bundles (which some of the Translator functions are just wrappers of).

#### Accessing Component Class Members

##### Getting

**Setting**

### Examples

## Using Non-Loom Compenents

This section explains how you might go about using non-Loom sensors with the rest of Loom functionality.

If you are not integrating you sensor into Loom, keep your code in the .ino file or any additional files you have made. 

#### **Components**

Components your code will likely have are:

- Include sensor library
- Possible instance of sensor class
- Setup
- Measure sensor

All of this can be done as normal, as if you were not using Loom. To get the data of your sensor to work with the rest of Loom, you will need to add the data to an OSC bundle.

#### Adding to an OSC Bundle

The package_bundle() API function populates the provided bundle. To add your own data to this bundle you can use the provided loom_translator functions, namely:

- Use `append_to_bundle_key_value(OSCBundle *bndl, char* key, T elem)`. This function adds a new message to the bundle, with the key at the end of the address, and with a single data point, `elem`.
- Use `append_bundle`. This function is overloaded and uses templates, so it can take most data types in either singular or array formats, but simply adds to the first message in the bundle - which may or may not be desirable. 

- Directly use any of the [OSC API](https://github.com/CNMAT/OSC/blob/master/API.md).

Most of the time Loom OSC bundles are in key-value pairs, that is, 1 message per data point, with the one element being the data point.
