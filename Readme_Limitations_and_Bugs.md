# Project Loom: Limitations and Bugs

## *To be updated*



## Bugs

## Configuration Conflicts

The following combinations of options in the configuration file result in various conflicts (e.g. of dependencies) that are known to result in errors or undefined behavior. 

## Other Notes

#### nRF Libraries

The libraries for nRF support - [RF24](https://github.com/nRF24/RF24) and [RF24Network](https://github.com/nRF24/RF24Network) - need to be the optimized forks, not the defaults provided the Arduino library manager. The correct versions are provided in the [Loom dependencies](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Dependencies) folder.

#### EnableInterrupt.h and RH\_RF95.h compatibility

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

```cpp
#define EI_NOTEXTERNAL
```

###