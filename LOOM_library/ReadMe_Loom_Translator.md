# Project Loom: Translator

The Loom Translator (or converter) is used to convert data in one format into another format, for best fit to the task at hand. The functions for the converter currently live in the loom_OSC_translator.h file of the Loom Library.

## Table of Contents



## Supported Formats 

-  OSC Bundle
   -  Unspecified format
   -  Key-value format via multiple messages
   -  Key-value format via a single message
-  OSC Message
   -  *Limited support – OSC bundles with single messages preferred*
-  Arrays
   -  Formats
      -  Unspecified format
      -  Interleaved key-values format: [key1, value1, key2, value2, ...]
      -  Associated array, key and values format: [key1, key2, ...], [value1, value2, ...]
   -  Data Types
      -  Ints
      -  Floats
      -  Strings
-  Single elements
   -  *There is not much conversion here, but there will be functions to add elements to the other formats*


## Available Conversions

| Source Format                        | Destination Format                   | Function | Notes |
| ------------------------------------ | ------------------------------------ | -------- | ----- |
| **Bundle-Bundle Conversions**        |                                      |          |       |
| Bundle (key-value, multi-msg)        | Bundle (key-value, single-msg)       |          |       |
| Bundle (key-value, single-message)   | Bundle (key-value, multi-msg)        |          |       |
| **Bundle–String Conversions**        |                                      |          |       |
| Bundle (any-form)                    | string                               |          |       |
| string                               | Bundle                               |          |       |
| **Bundle–>Array Conversions**        |                                      |          |       |
| Bundle (key-value, single/multi-msg) | Array (key-value, interleaved)       |          |       |
| Bundle (key-value, single/multi-msg) | Arrays (key-value, associated)       |          |       |
| **Array->Bundle Conversions**        |                                      |          |       |
| Array (key-value, interleaved)       | Bundle (key-value, single/multi-msg) |          |       |
| Arrays (key-value, associated)       | Bundle (key-value, single/multi-msg) |          |       |
| **Array–Array Conversions**          |                                      |          |       |
| Array (key-value, interleaved)       | Arrays (key-value, associated)       |          |       |
| Arrays (key-value, associated)       | Array (key-value, interleaved)       |          |       |

### Conversion Notes

**In Place Conversion**

Some of the conversions that keep the same data structure just organized in a different way (namely between bundles with single and multiple messages) can optionally be done 'in-place'. The default means of calling the conversion functions is to provide a source data structure, and a destination data structure, the later of which will be emptied if not already, and filled with the translated data. The source data structure will remain unchanged

If instead, one omits the destination structure (to a function that supports it), the source data structure itself will be filled with the translated data. 

## Details about OSC Bundles

The OSC library ReadMe and API documentation, [here](https://github.com/CNMAT/OSC), are very helpful for getting started. The details below focus primarily on the Loom Library usage of the protocol.

### Supported Data Types

The Loom Translator currently only supports conversions with data that is of type int, float, or string. Messages and bundles can still be made and sent via WiFi with the full Arduino OSC supported set of data types.

- Int
- Float
- String

### Messages

OSC messages take the form:

```
/message/address arg1 arg2 agr3 ...
```

The address can be used for routing the address, i.e. only acting on messages with the address that a device is expecting.

### Bundles

Bundle are simple a group of messages, potentially with a timestamp.

The translator currently supports three structures of bundles. Note that there is nothing enforcing these formats, so 'improperly' formatted bundles, while still valid bundles, will likely see undefined behavior when trying to convert to another data format.

**Bundle: Unspecified Format**

This is just a standard bundle without any assumed structure / meaning to the address or arguments. Conversions in this format are somewhat limited. Conversions to and from strings is fully supported, but most other translations have undefined behavior (but might work, feel free to experiment).

**Bundle: Key-Value Single Message**

This bundle structure is a single message followed by all of the data, assumed to be interleaved keys and values. This is closer to the format used by outputs such as PushingBox.

```
Address 1: /LOOM/Device7/data "abc" 1 "def" 2.34 "ghi" "five"
```

**Bundle: Key-Value Multiple Messages** 

This bundle structure is multiple messages, the end of each address being a key, and each message having a single argument, the corresponding value of the pair. This is commonly the format expected by Max/MSP for simplest routing.

```
Bundle Size: 3
Address 1: /LOOM/Device7/abc
Value 1: 1
Address 2: /LOOM/Device7/def
Value 1: 2.34
Address 3: /LOOM/Device7/ghi
Value 1: "five"
```

## Details about Arrays

There are three formats of arrays supported by the translator, with a focus on the two key-value formats. The formats are: unspecified, key-value interleaved, key-value associated arrays.

To match the bundles, three data types are permitted: ints, floats, and strings.

**Array: Unspecified**

This array format does not have as much support as the key-value arrays, but can be converted to an unspecified format bundle with a single message

**Array: Key-Value Interleaved**

This array format closely corresponded to the key-value, single message bundle format. It is a flat array that is assumed to be alternating keys and values:

```
[key1, value1, key2, value2, key3, value3]
```

**Array: Key-Value Associated Arrays**

This format uses two associated arrays (expected to be the same size), one of keys and the other of the corresponding values:

```
[key1,   key2,   key3]
[value1, value2, value3]
```

### Conversion from Array to Bundle

When converting to a bundle from an array, there are options to try to interpret the individiual elements as one of three supported data types, as bundles, unlike arrays (in Arduino C/C++), can support mixed data types. This can also be forced, in which un-convertable data becomes something like 0, or 0.0, or "".

These settings are controlled by the optional ```interpret``` parameter. If omitted to the function calls, interpret defaults to the recommended settings of 0.

####Encoding of Interpret Settings

**0 - 3 assume elements are in [key1, value1, key2, value2...] format:**

- Even indexes being keys, and will be left as strings
- Odd indexes being data, and will be interpreted as specified

**4-6 do NOT assume key value pairs and will also interpret even indexes as specified, use with caution**

- Note: likely will be some issues when making multi-message bundles with below options, though single message format works fine

| Setting | Conversion Meaning               | Comments                                   |
| ------- | -------------------------------- | ------------------------------------------ |
| 0       | Smart (default if not specified) | Data Int->int, float->float, other->string |
| 1       | Int                              | Non-int data will become 0                 |
| 2       | Float                            | Non-float data will become 0.0             |
| 3       | String                           | Leaves/converts all to strings             |
| 4       | Smart–All                        | Int->int, float->float, other->string      |
| 5       | Int–All                          | Non-int wsill become 0                     |
| 6       | Float–All                        | Non-float will become 0.0                  |

## Details about Bundle-String Translator 

Open Sound Control (OSC) is the transmission protocol used by Project Loom.  The
Arduino OSC implementation, along with more information about OSC,
can be found [here](https://github.com/CNMAT/OSC).  The functions to convert between strings and OSC bundles was formerly referred to as the OSC interpreter (and was the base translator), and any references to that name specifically means these conversions.

While OSC Bundles can be sent directly using WiFi, bundles must be reencoded to
transmit them via LoRa or nRF.  The OSC Interpreter allows OSC Bundles to be
translated into strings and allows strings to be translated back into OSC Bundles.
Bundles are encoded by taking each message address, along with corresponding data 
values, are concatenated into a comma delimited string, and all messages in the 
bundle are concatenated into a space delimited string.  

Currently, only three data values are supported by the interpreter: int32\_t, 
float, and C strings.  The following shows how the supported data values are 
encoded:

| Type        | Encoding                         | Example Input  | Example Encoding  |
|:-----------:|:--------------------------------:|:--------------:|:-----------------:|
| int32\_t    | 'i' + raw bits to unsigned long  | 12001          | i12001            |
| float       | 'f' + raw bits to unsigned long  | 6.0            | f1086324736       |
| c string    | 's' + string                     | Hello          | sHello            |

Let's look at an example of how an entire bundle is encoded.  Here is the original bundle:

```
OSCBundle
    Message 1 Address: '/LOOM/D1'
        Data 1: (float) 6.0
        Data 2: (int32_t) 84
    Message 2 Address: '/LOOM/D2'
        Data 1: (c string) 'temp'
```

Here is what that same bundle looks like when encoded with the interpreter:

```
'/LOOM/D1,f1086324736,i84 /LOOM/D2,stemp'
```

**NOTE:** The OSC Interpreter does not currently support the encoding of OSCBundles which
contain spaces or commas in either message addresses or string data values.

## OSC Issues

OSC Bundles can cause some difficult to diagnose issues.  One of the main issues we have
encountered is that multiple OSC Bundles can cause programs to run once through a loop but
stop on the second iteration through a loop.  Below are some methods to avoid some of these
issues (or look at the loom_OSC_translator.h file for our method implementation):

**Avoid Declaring Multiple OSC Bundles**

Declaring multiple OSC bundles can cause both the M0 and the 32u4 to hang.  The best practice
for sending multiple OSC bundles is to declare a single bundle object and empty it in between
uses.

DO:

``` cpp
void loop() {
    OSCBundle bndl;
    bndl.add('addr1').add(6.0);

    // Do whatever you want with bundle 1 here

    bndl.empty();
    bndl.add('addr2').add('data').add('more-data');

    // Do whatver you want with bundle 2 here
}
```

DON'T:

``` cpp
void loop() {
    OSCBundle bndl1;
    OSCBundle bndl2; // Declaring multiple OSC bundles is BAD!
    bndl1.add('addr1').add(6.0);
    bndl2.add('addr2').add('data').add('more-data');
}
```

**Always Pass OSC Bundles to Functions By Pointers**

Passing OSC bundles by value can cause the Feather 32u4 to hang.  Furthermore, functions with 
an OSC bundle return type may causes issues as well.  Although the Feather M0 functions
properly when passing OSC bundles by value, it is still best practice to pass the address of the
OSC bundle instead.  

DO:

``` cpp
void package_data(OSCBundle *bndl) {
    // Whatever you want the function to do
}
```

DON'T:

``` cpp
OSCBundle package_data() {
    // OSCBundle return types are BAD!
}

void process(OSCBundle bndl) {
    // Passing OSCBundles by value is also BAD!
}
```