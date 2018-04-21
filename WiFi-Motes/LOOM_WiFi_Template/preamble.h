#include <OSCBundle.h> //all LOOM devices rely on the Arduino OSC library in some way

// Packet header creation macro
#define STR_(x) #x                // Helper function
#define STR(x) STR_(x)            // To concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE // Results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function

#define VBATPIN A7                // Pin to check for battery voltage
//#include "declarations_and_structures.h"

//------------------------------------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//------------------------------------------------------------------------------------------------------
#define MEM_FLASH 0
#define MEM_EEPROM 1  

// Still experimental
#ifdef __SAMD21G18A__
  #define is_m0
  #define MEM_TYPE MEM_FLASH
#endif
#ifdef __AVR_ATmega32U4__
  #define is_32u4
  #define MEM_TYPE MEM_EEPROM
#endif

#define button 10               // Using on-board button, specify attached pin, transmitting

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif

#ifdef is_sleep_interrupt
  #include <LowPower.h>           // Include this if transmitting on pin interrupt
#endif

// Using Config defines, check which headers to include
#if DEBUG == 1
	#include <SPI.h>
#endif

#if is_analog > 0
  #include "subroutines_analogIn.h"
#endif
#ifdef is_i2c 
  #include "subroutines_mpu6050.h"
#endif
#if is_neopixel == 1
  #include "subroutines_neopixel.h"
#endif
#if num_servos > 0
  #include "subroutines_servo.h"
#endif
#if is_relay == 1
  #include "subroutines_relay.h"
#endif
#if is_wifi == 1
	#include "subroutines_wifi.h"
#endif
#if is_lora == 1
	#include "subroutines_lora.h"
#endif
#include "subroutines_flash.h"

// These may refer to functions in above headers
#include "subroutines_common.h"
