#define FAMILY "LOOM"     // Should always be "LOOM", you can change this if you are setting up your own network
//#define DEVICE "Ishield" // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "RelayShield" or "IShield"
#define INIT_INST 1


#define REQUEST_SETTINGS 0

//--OPTIONS--//

#define LOOM_DEBUG 0	  // Set to 1 if you want Serial statements from various functions to print

#define is_wifi    1
#define is_lora    0

#define is_ishield 1
#define num_servos 0
#define num_steppers 0
#define is_relay   0
#define num_analog  2
#define is_decagon 0
//Multiplexer
#define is_tca9548a 0

#if is_tca9548a == 1
	#define UPDATE_PERIOD 5000 //milliseconds
	//Lux Sensor
	#define is_tsl2591 1

	//Accelerometer / Magnetometer
	#define is_fxos8700 1

	//Gyroscope
	#define is_fxas21002 1

	//ZX_Distance Sensor
	#define is_zxgesturesensor 1

	//Temperature / Humidity
	#define is_sht31d 1

	//Sonar
	#define is_mb1232 1

	//Accelerometer / Gyroscope
	#define is_mpu6050 1
#endif

#if is_ishield == 1
	#define is_neopixel 1      // Toggle based on whether Neopixels are being used

	#define is_mpu6050  1

//	#define button 10

	#if is_neopixel == 1
		#define NEO_0 false
		#define NEO_1 false
		#define NEO_2 true
	#endif  
#endif

#if is_wifi == 1
	#define DEFAULT_MODE      WPA_CLIENT_MODE //AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
	#define DEFAULT_NETWORK   "OPEnS"
	#define DEFAULT_PASSWORD  "arduino101"
	#define COMMON_PORT       9440
	#define INIT_PORT         9441
#endif

// LoRa Device Type
// 0: Hub, 1: Node, 2 = Repeater
#if is_lora == 1
	#define lora_bundle_fragment 0		// splits bundles into smaller bundles to avoid overflowing size LoRa can send

	#define lora_device_type 0
	
	#define SERVER_ADDRESS 0          	// Use 0-9 for SERVER_ADDRESSes
	#define RF95_FREQ      915.0      	// Hardware specific, Tx must match Rx

	#if lora_device_type == 0 // Hub
		#define NUM_FIELDS 32           // Maximum number of fields accepted by the PushingBox Scenario    
		#include <Ethernet2.h>			// -- ideas on how to move this to declarations? (its needed for IPAddress but that is a user option)
		
//		String data[NUM_FIELDS];
		char device_id[]   = "v25CCAAB0F709665";     // Required by PushingBox, specific to each scenario
		char server_name[] = "api.pushingbox.com";
	
		//Use this for OPEnS Lab
		byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6};
		IPAddress ip(128,193,56,138); 
//		EthernetClient client;            
	#endif
	
	#if lora_device_type == 1 // Node
		#define CLIENT_ADDRESS 10       //10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS, 
	#endif														//10-19 for 0, 20 - 29 for 1, etc.
	
#endif

#if is_lora == 0
	#define is_sleep_period 80          // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
										// Do not use with LoRa
#endif
//#define is_sleep_interrupt 11       	// Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt





// Make sure only one device type is enabled
#if ( (is_ishield) + (num_servos > 0) + (num_steppers > 0) + (is_relay) + (is_decagon) + (is_tca9548a) ) > 1
	device_type_error // this will force an error
#endif

// Automatically set device name
#if   is_ishield
	#define DEVICE "Ishield"
#elif num_servos
	#define DEVICE "ServoShield"
#elif num_steppers
	#define DEVICE "StepperShield"
#elif is_relay
	#define DEVICE "RelayShield"
#elif is_decagon
	#define DEVICE "Decagon"
#elif is_tca9548a
	#define DEVICE "MuxShield"
#else
	#define DEVICE "Generic"
#endif


