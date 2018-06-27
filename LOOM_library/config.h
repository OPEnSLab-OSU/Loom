// ================================================================ 
// ===                         CONFIG                           === 
// ================================================================
//
// Use this file to configure the device you are flashing the firmware of
//
// This file is divided into two sections:
//
//  Options: The top half that can be modified to change common settings
//
//  Advanced Options: the bottom half that is at least partially set based on  
//    the top half but may be modified for more specific behavior. 
//    Also contains other specific options that don't need to change usually
//
// defines that should not be modified by the user are
//   located in the library and are not present here



// ================================================================ 
// ===                         OPTIONS                          === 
// ================================================================

// --- Device Identification --- 
#define FAMILY "LOOM"		// Will usually be "LOOM", you can change this if you are setting up your own network
#define CHANNEL          7	// Channel to use. Set to 1-8 for channels A-H, respectively. Alternatively can define to -1 to used advanced option INIT_INST
#define REQUEST_SETTINGS 0	// 1 to request channel settings from Max Channel Manager, 0 to not
#define AUTO_NAME        1	// 1 to enable naming device based on configured settings (if not set manual name in advanced options)

// --- Debugging --- 
#define LOOM_DEBUG   1		// Set to 1 if you want Serial statements from various functions to print
							// NOTE: serial monitor must be opened for device to setup
							// LOOM_DEBUG_Print* are Serial prints that are removed if debugging is off

// --- Enabled Communication Platform(s) --- 
#define is_wifi      1		// 1 to enable WiFi
#define is_lora      0		// 1 to enable LoRa (cannot be used with nRF) (Further customization in advanced options)
#define is_nrf       0		// 1 to enable nRF (cannot be used with LoRa) (Further customization in advanced options)

#define is_sd         0		// 1 to enable SD card 
#define is_pushingbox 0     // 1 to enable PushingBox (currently requires Ethernet) (Auto enabled if using LoRa hub) (currently does not appear to work with WiFi)
#define is_adafruitio 0		// 1 to enable Adafruit IO (currently requires WiFi)

// --- Enabled Actuators --- 
#define num_servos   0		// Number of servos being used
#define num_steppers 0		// Number of stepper motors being used
#define is_relay     0		// 1 if relays are being used (enables two, on pins 5 and 6)

// --- Enabled Sensors --- 
#define num_analog  0		// Number of analog inputs being used (0=None ; 1=A0 ; 2=A0,A1 ; 3=A0,A1,A2)
#define is_decagon  0		// 1 if GS3 Decagon is being used
#define is_tca9548a 0		// 1 if Multiplexer is being used. (Further customization in advanced options)

// --- Prebuilt Devices ---
#define is_ishield  0		// 1 to specify using Ishield (should enable only wifi as communication platform)



// --- WiFi Settings ---
// Requires is_wifi to be set to 1 to take effect
#if is_wifi == 1
	#define DEFAULT_MODE      WPA_CLIENT_MODE	// AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
	#define DEFAULT_NETWORK   "OPEnS"			// Network SSID / name
	#define DEFAULT_PASSWORD  "Replace_with_your_wifi_password"		// Network password
#endif

// ================================================================ 
// ===                    ADVANCED OPTIONS                      === 
// ================================================================

// --- Set Instance Number and UDP Port
// (do not modify the A-H cases, only the X case, if not using channels)
// If using channels 
#if   CHANNEL == 1
	#define INIT_INST 1
	#define INIT_PORT 9441
#elif CHANNEL == 2
	#define INIT_INST 2
	#define INIT_PORT 9442
#elif CHANNEL == 3
	#define INIT_INST 3
	#define INIT_PORT 9443
#elif CHANNEL == 4
	#define INIT_INST 4
	#define INIT_PORT 9444
#elif CHANNEL == 5
	#define INIT_INST 5
	#define INIT_PORT 9445
#elif CHANNEL == 6
	#define INIT_INST 6
	#define INIT_PORT 9446
#elif CHANNEL == 7
	#define INIT_INST 7
	#define INIT_PORT 9447
#elif CHANNEL == 8
	#define INIT_INST 8
	#define INIT_PORT 9448

// If not using channels, then the following intial port will be used
#elif CHANNEL == -1
	#define INIT_INST    3	// Initial device instance number (normally 1-8 when using channels)
	#define INIT_PORT 9443	// Initial device UDP port (normally 1-8 when using channels)
#endif


// --- WiFi UDP Ports ---
#if is_wifi == 1
	#define COMMON_PORT     9440	// Expected by Max to be 9440, don't change unless using custom Max patches
#endif



// --- Set Device Name ---
#if AUTO_NAME == 1
	// Make sure only one device type is enabled
	#if ( (is_ishield) + (num_servos > 0) + (num_steppers > 0) + (is_relay) + (is_decagon) + (is_tca9548a) ) > 1
		autoname_device_type_error	// this will force an error if too many sensor/actuator were defined (needs to be < 1 for autoname to work)
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
		#define DEVICE "Unknown"
	#endif
#else
	// --- Custom Device Identification --- 
	#define DEVICE "Generic" // The device name (can be changed), used only if not using automatic device name
#endif



// --- Multiplexer Options ---
#if is_tca9548a == 1
	#define UPDATE_PERIOD 5000		// Milliseconds between multiplexer sensor list being updated
	
	// 1 to enable sensor type
	#define is_tsl2591  1			// Lux Sensor
	#define is_fxos8700 1			// Accelerometer / Magnetometer
	#define is_fxas21002 1			// Gyroscope
	#define is_zxgesturesensor 1	// ZX_Distance Sensor
	#define is_sht31d 1				// Temperature / Humidity
	#define is_mb1232 1				// Sonar
	#define is_mpu6050 1			// Accelerometer / Gyroscope
#endif


// --- Ishield Options ---
#if is_ishield == 1
	#define is_mpu6050  0			// Enables MPU6050 on Ishield
	#define is_neopixel 1			// Toggle based on whether Neopixels are being used 
	// #define button 10			// Usually automatically defined, uncomment if that does not happen		

	#if is_neopixel == 1			// Which Ishield ports to enable Neopixels for 
		#define NEO_0 0				// Port 0 (A0, closest to end of Ishield)
		#define NEO_1 0				// Port 1 (A1, middle port)
		#define NEO_2 1				// Port 2 (A2, port closest to MPU6050)
	#endif  
#endif

// --- LoRa Options ---
#if is_lora == 1
	#define lora_device_type     0 		// 0: Hub, 1: Node, 2 = Repeater
	#define lora_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send

	#define SERVER_ADDRESS 0			// Use 0-9 for SERVER_ADDRESSes
	#define RF95_FREQ      915.0		// Hardware specific, Tx must match Rx

	#if lora_device_type == 0
		#define is_pushingbox 1
	#endif
	#if lora_device_type == 1 			// If Node
		#define CLIENT_ADDRESS 10		// 10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS, 
	#endif								// 10-19 for 0, 20 - 29 for 1, etc.
#endif

// --- nRF Options --- 
#if is_nrf == 1
	#define nrf_device_type     0 		// 0: Hub, 1: Node, 2 = Repeater
	#define nrf_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send
	
	#define NRF_HUB_ADDRESS 1			// Use 0-9 for SERVER_ADDRESSes
	
	#if nrf_device_type == 1
		#define NRF_NODE_ADDRESS 0
	#endif
#endif

// --- PushingBox Options ---
#if is_pushingbox == 1			
	#define NUM_FIELDS 32			// Maximum number of fields accepted by the PushingBox Scenario    
	#include <Ethernet2.h>			// (this is needed for IPAddress, do not remove)
	
	// String data[NUM_FIELDS];
	char device_id[]   = "vF8786ECBD85A1AE";	// Required by PushingBox, specific to each scenario
	char server_name[] = "api.pushingbox.com";	// PushingBox server, probably don't need to change

	//Use this for OPEnS Lab
	byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6}; 
	IPAddress ip(128,193,56,138); 
#endif

// --- Adafruit IO Options
#if is_adafruitio == 1
	#define AIO_USERNAME    "Goertzel"
	#define AIO_KEY         ""
#endif



// --- Delay between loops
#if is_lora == 0 						// Cannot not use with LoRa
	#define is_sleep_period 80			// Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. 
										// Change the value according to the length of your desired transmission interval
										// 80 seems to be a good amount, around 50 and lower may result in lost packets over WiFi
#endif
// #define is_sleep_interrupt 11			// Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt




















