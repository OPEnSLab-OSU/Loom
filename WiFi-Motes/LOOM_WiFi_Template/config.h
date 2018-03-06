#define FAMILY "LOOM"    // Should always be "LOOM", you can change this if you are setting up your own network
#define DEVICE "Ishield"  // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "RelayShield" or "IShield"





//--OPTIONS--//
#ifndef DEBUG
#define DEBUG 1 //set to 1 if you want Serial statements from various functions to print
#endif

#define SEND_OSC          // Comment this out to turn off sending of OSC messages
#define RECEIVE_OSC       // Comment this out to turn off receiving of OSC messages

//#define is_servo

#define is_analog 2
#define is_i2c 0x86
#ifdef is_i2c
  #define is_mpu6050
  #define is_neopixel
#endif
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting 
#define VBATPIN A7       // Pin to check for battery voltage

#define is_sleep_period 50 // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
//#define is_sleep_interrupt 11 // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt

#define INIT_PORT 9436
#define INIT_INST 0
#define AP_NAME "featherM0"

