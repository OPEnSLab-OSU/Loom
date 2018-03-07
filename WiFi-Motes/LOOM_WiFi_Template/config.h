#define FAMILY "LOOM"    // Should always be "LOOM", you can change this if you are setting up your own network
#define DEVICE "ServoShield"  // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "RelayShield" or "IShield"





//--OPTIONS--//
#ifndef DEBUG
#define DEBUG 0 //set to 1 if you want Serial statements from various functions to print
#endif

#define DEFAULT_MODE WPA_CLIENT_MODE //AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
#define DEFAULT_NETWORK "OPEnS"
#define DEFAULT_PASSWORD "arduino101"

#define is_servo
#ifdef is_servo
  #define num_servos 1
#endif
//#define is_relay

#define A0            14 
#define A1            15 
#define A2            16


//#define is_analog 2
//#define is_i2c 0x86
#ifdef is_i2c
  #define is_mpu6050
  #define is_neopixel
#endif

#ifdef is_neopixel
  #define NEO_0 false
  #define NEO_1 false
  #define NEO_2 true
#endif

#ifndef is_relay
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting
#endif
#define VBATPIN A7       // Pin to check for battery voltage

#define is_sleep_period 50 // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
//#define is_sleep_interrupt 11 // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt

#define INIT_PORT 9422
#define INIT_INST 2
#define AP_NAME "featherM0-2"

