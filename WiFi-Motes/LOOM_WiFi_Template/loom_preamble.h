// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <OSCBundle.h> //all LOOM devices rely on the Arduino OSC library in some way


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// Packet header creation macro
#define STR_(x) #x                // Helper function
#define STR(x) STR_(x)            // To concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE // Results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function

#define VBATPIN A7                // Pin to check for battery voltage
//#include "declarations_and_structures.h"


//---------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//---------------------------------------------------------------------------
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



// ================================================================ 
// ===                  INCLUDE DEVICE FILES                    === 
// ================================================================
#if is_analog > 0
  #include "loom_analogIn.h"
#endif
#if is_neopixel == 1
  #include "loom_neopixel.h"
#endif
#if num_servos > 0
  #include "loom_servo.h"
#endif
#if is_relay == 1
  #include "loom_relay.h"
#endif
#if is_wifi == 1
	#include "loom_wifi.h"
#endif
#if is_lora == 1
	#include "loom_lora.h"
#endif
#if is_mpu6050 == 1
  #include "loom_mpu6050.h"
#endif
#if is_max31856 == 1
  #include "loom_max31856.h"
#endif
#if is_tsl2591 == 1
	#include "loom_tsl2591.h"
#endif
#if is_fxos8700 == 1
	#include "loom_fxos8700.h"
#endif
#if is_fxas21002 == 1
	#include "loom_fxas21002.h"
#endif
#if is_zxgesturesensor == 1
	#include "loom_zxgesturesensor.h"
#endif
#if is_sht31d == 1
	#include "loom_sht31d.h"
#endif
#if is_mb1232 == 1
	#include "loom_mb1232.h"
#endif
#if is_tca9548a == 1
	#include "loom_tca9548a.h"
#endif



#include "loom_flash.h"
#include "loom_common.h"  // These may refer to functions in above headers
