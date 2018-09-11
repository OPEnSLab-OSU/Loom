# Project Loom: LoRa

This is the Section corresponding to the LoRa flavor of wireless COM for IoA.
Supports both the Adafruit Feather M0 and Adafruit Feather 32u4.

## Dependencies
### LoRa Libraries:
* [RadioHead](https://github.com/adafruit/RadioHead)

### Other Libraries:
* [Enable Interrupt](https://github.com/GreyGnome/EnableInterrupt)
* [Extended RTC Library](https://github.com/FabioCuomo/FabioCuomo-DS3231)
* [Low-Power](https://github.com/rocketscream/Low-Power)
* [OSC Arduino Library](https://github.com/CNMAT/OSC)
* [SDI-12 Functionality](https://github.com/EnviroDIY/Arduino-SDI-12)

## Troubleshooting

### Supported SDI-12 Pins

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

### EnableInterrupt.h and RH\_RF95.h compatibility

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

``` cpp
#define EI_NOTEXTERNAL
```

### Using RTCLibExtended.h with the Adafruit Feather M0

The RTC extended library utilizes specific macros that are defined by default for
the Adafruit Feather 32u4 but not for the Feather M0.  To use the RTC extended 
library with the Feather M0, the following line must be added to the 
RTClibExtended.h file:

``` cpp
#define _BV(bit) (1 << (bit))
```

### 
