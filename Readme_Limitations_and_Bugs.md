# Project Loom: Limitations and Bugs

## 32u4 Limitations

The 32u4 has a number of limitations in comparision to the M0 processors, which are documented below.

**Program Storage**

The 32u4 has limited program storage, generally not enough to fit a moderately complex Loom project.

**Issues with Floats**

When compiling for the 32u4, the Arduino IDE seems to use a smaller subset of standard C functions. This is most apparent when using floats. For example, `strtof` is not provided (but is for M0), and `sprintf` does not work with floats (often requiring cumbersome work-arounds / using the String library).

**RAM**

It seems that exceeding the provided RAM causes freezing. In the LOOM library, it is not unusual for for a 32u4 to run normally until receiving a bundle, likely due to declaration of more variables exceeding RAM.

## Bugs

**MPU6050 library bug** 

The default MPU6050 library has a bug preventing the alternate I2C address (0x69 instead of 0x68) from being used. Namely, line 375 of 'MPU6050_6Axis_MotionApps20.h' needed to be changed from:

    setSlaveAddress(0, 0x68);
to:

```
setSlaveAddress(0, devAddr);
```

## Configuration Conflicts

The following combinations of options in the configuration file result in various conflicts (e.g. of dependencies) that are known to result in errors or undefined behavior. Such conflicts are also mentioned in the config.h file itself.

- LoRa + nRF
- is_sleep_period + LoRa (Adafruit_ASFcore-master/tc_interrupt.c:140: multiple definition of 'TC3_Handler')

## Other Notes

#### nRF Libraries

The libraries for nRF support - [RF24](https://github.com/nRF24/RF24) and [RF24Network](https://github.com/nRF24/RF24Network) - need to be the optimized forks, not the defaults provided the Arduino library manager. The correct versions are provided in the [Loom dependencies](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Dependencies) folder.

#### SDI-12 with 32u4

 To use the SDI-12 library with the 32u4, the user must define their own
pin interrupts using the Enable Interrupt library.  The following line from
the file SDI12.h must be uncommented so that external interrupts can be defined:

```cpp
#define SDI12_EXTERNAL_PCINT // uncomment to use your own PCINT ISRs
```

#### EnableInterrupt.h and RH\_RF95.h compatibility

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

```cpp
#define EI_NOTEXTERNAL
```

#### Using RTCLibExtended.h with the Adafruit Feather M0

The RTC extended library utilizes specific macros that are defined by default for
the Adafruit Feather 32u4 but not for the Feather M0.  To use the RTC extended 
library with the Feather M0, the following line must be added to the 
RTClibExtended.h file:

```cpp
#define _BV(bit) (1 << (bit))
```

### 