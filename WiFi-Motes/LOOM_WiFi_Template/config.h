#define FAMILY "LOOM"         // Should always be "LOOM", you can change this if you are setting up your own network
#define DEVICE "Ishield"  // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "RelayShield" or "IShield"
#define INIT_INST 2


//--OPTIONS--//
#define DEBUG 1   // Set to 1 if you want Serial statements from various functions to print
#define num_servos 1
//#define is_relay


//------------------------------------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//------------------------------------------------------------------------------------------------------
#define MEM_FLASH 0
#define MEM_EEPROM 1

#ifdef __SAMD21G18A__
	#define is_m0
  #define MEM_TYPE MEM_FLASH
#endif
#ifdef __AVR_ATmega32U4__
	#define is_32u4
  #define MEM_TYPE MEM_EEPROM
#endif


#define is_wifi
#ifdef is_wifi
	#define DEFAULT_MODE      WPA_CLIENT_MODE //AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
	#define DEFAULT_NETWORK   "OPEnS"
	#define DEFAULT_PASSWORD  "arduino101"
	#define INIT_PORT 9422
#endif

//#define is_lora
#ifdef is_lora
  #define is_hub
//#define is_node

  #define SERVER_ADDRESS 0							//Use 0-9 for SERVER_ADDRESSes
  
  #ifdef is_node
    #define CLIENT_ADDRESS 10							//10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS, 
  #endif																	//10-19 for 0, 20 - 29 for 1, etc.
	
	#define RF95_FREQ 915.0						//Hardware specific, Tx must match Rx
	
  #ifdef is_hub
    #define NUM_FIELDS 16			      //Maximum number of fields accepted by the PushingBox Scenario
  #endif
#endif



#define is_analog 2
#define is_i2c 0x86
#ifdef is_i2c
  #define is_mpu6050
  #define is_neopixel
#endif

#ifdef is_neopixel
  #define NEO_0 false
  #define NEO_1 false
  #define NEO_2 true
#endif


// Dont quite recall how Chet wanted this
//#ifdef is_mpu6050
//  #define is_i2c 0x86
//#endif





//#define is_sleep_period 50          // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
//#define is_sleep_interrupt 11       // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt




