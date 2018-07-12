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

// --- Enabled Communication Platform --- 
#define is_wifi      0 		// 1 to enable WiFi
#define is_lora      1		// 1 to enable LoRa (cannot be used with nRF) (Further customization in advanced options)
#define is_nrf       0		// 1 to enable nRF (cannot be used with LoRa) (Further customization in advanced options)
#define is_ethernet  1      // 1 to enable Ethernet (a number of options below might enable this anyway though)
#define is_fona      0      // Fona is currently being integrated
#define is_bluetooth 0      // Sorry, Bluetooth is not implemented yet

// --- Additional Platforms ---
#define is_pushingbox 1     // 1 to enable PushingBox (currently requires Ethernet) (Auto enabled if using LoRa hub) (currently does not appear to work with WiFi)
#define is_adafruitio 0		// 1 to enable Adafruit IO (currently requires WiFi)

#define is_sd         1		// 1 to enable SD card 
#define is_rtc        1		// Enable RTC functionality

// --- Device Telemetry Type ---
// #define hub_node_type     1 		// 0: Hub, 1: Node, 2 = Repeater   (this is going to be removed in the future, replaced with the following 3 options)
#define is_hub       0
#define is_node      1
#define is_repeater  0		// Sorry, this doesn't do anything yet

// --- Enabled Actuators --- 
#define num_servos   0		// Number of servos being used
#define num_steppers 0		// Number of stepper motors being used
#define is_relay     0		// 1 if relays are being used (enables two, on pins 5 and 6)

// --- Enabled Sensors --- 
#define num_analog   0		// Number of analog inputs being used (0=None ; 1=A0 ; 2=A0,A1 ; 3=A0,A1,A2)
#define is_decagon   0		// 1 if GS3 Decagon is being used
#define is_tca9548a  0		// 1 if Multiplexer is being used. (Further customization in advanced options)

// --- Prebuilt Devices ---
#define is_ishield   0		// 1 to specify using Ishield (should enable only wifi as communication platform)
#define is_sapflow   1


// --- WiFi Settings ---
// Requires is_wifi to be set to 1 to take effect
#if is_wifi == 1
	#define DEFAULT_MODE      WPA_CLIENT_MODE	// AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
	// #define DEFAULT_NETWORK   "OPEnS"			// Network SSID / name
	// #define DEFAULT_PASSWORD  "Replace_with_your_wifi_password"		// Network password
	#define DEFAULT_NETWORK   "Visitor"			// Network SSID / name
	#define DEFAULT_PASSWORD  ""		// Network password
#endif

// ================================================================ 
// ===                    ADVANCED OPTIONS                      === 
// ================================================================

// --- Set Instance Number and UDP Port
// (do not modify the A-H cases, only the X case, if not using channels)
// If not using channels, then the following intial port will be used
#if CHANNEL == -1
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
	#elif num_servos > 0
		#define DEVICE "ServoShield"
	#elif num_steppers > 0
		#define DEVICE "Stepper"
	#elif is_relay
		#define DEVICE "RelayShield"
	#elif is_decagon
		#define DEVICE "Decagon"
	#elif is_tca9548a
		#define DEVICE "MuxShield"
	#elif is_sapflow
		#define DEVICE "Sapflow"
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
	#define is_tsl2591         1	// Lux Sensor
	#define is_fxos8700        1	// Accelerometer / Magnetometer
	#define is_fxas21002       1	// Gyroscope
	#define is_zxgesturesensor 1	// ZX_Distance Sensor
	#define is_sht31d          1	// Temperature / Humidity
	#define is_mb1232          1	// Sonar
	#define is_mpu6050         1	// Accelerometer / Gyroscope
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
	#define LORA_HUB_ADDRESS 0			// Use 0-9 for SERVER_ADDRESSes
	#define RF95_FREQ      915.0		// Hardware specific, Tx must match Rx

	#if is_node == 1 					// If Node
		#define NODE_ADDRESS 10			// 10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS, 
	#endif								// 10-19 for 0, 20 - 29 for 1, etc.
	
	#define lora_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send
#endif

// --- nRF Options --- 
#if is_nrf == 1
	#define NRF_HUB_ADDRESS 1			// Use 0-9 for SERVER_ADDRESSes
	
	#if is_node == 1
		#define NRF_NODE_ADDRESS 0
	#endif

	#define nrf_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send
#endif


// --- FONA Options ---
#if is_fona == 1
	#define LOOM_DEBUG 1	// This is just temporary for now, as all Fona tests are done interactively through serial monitor
	#define fona_test  1    // This enables the Fona interactive testing Loop, note that this will override normal 
							//  Loom loop behavior until command to exit testing loop is issued
	#define fona_type 2G 	// Can be '2G' (808) or '3G' (5320A), currently only 2G is supported
#endif


// --- Hub Options ---
#if is_hub == 1
	#define is_ethernet   1	// not necessarily always true
	#define is_pushingbox 1 // only if Ethernet, WiFi, or cellular

	// The following two defines are planned to be implemented,
	// but are not in use currently
	// #define hub_input  LORA
	// #define hub_output ETHERNET
#endif


// --- Sapflowmeter Options ---
#if is_sapflow
	// #define hub_node_type 0          // 0: hub, 1: node
	#define is_lora       1          // enable LoRa
	#define is_rtc        1
	#define is_sd         1


	#if is_hub == 1
	 	#define is_ethernet   1
	 	#define is_pushingbox 1
	#endif

	#if is_node == 1
		#define num_analog    2      // two temperature sensors
    #define probe_type    0      // 0:TDM, 1: HRM
		#define heatpulse     2500   // For HRM probe heat pulse (e.g 2500:2.5 sec)
		#define is_sht31d     1      // Temperature / Humidity
		#define senddelay     60000   // send data every 1 min
	#endif
#endif 

// --- PushingBox Options ---
#if is_pushingbox == 1	

	// #define spreadsheet_id "16K7gOczeewt-wVHdnMR0ttWSrcqmVvWvG-2zJxo1-MA"	   // Google Spreadsheet ID 
	// #define spreadsheet_id "17XjrTjXENChYsHMVvKgePg8rsohwz0hyASkdeZZKROk"
	#define spreadsheet_id "***REMOVED***"
								// (found betweeen the "docs.google.com/spreadsheets/d/" and 
								// "/edit..." in the URL; random string of characters)
	#define tab_id  "15"   // Google Spreadsheet Sheet/Tab number. Sent as parameter to PushingBox/Google Scripts
								// can be number or string

	// char device_id[]   = "vF8786ECBD85A1AE";	// Required by PushingBox, specific to each scenario
	char device_id[]   = "***REMOVED***";



	// In the event that a device has multiple means of connecting to the internet (WiFi, Ethernet, and cellular), 
	// I'll probably have an order in which the device attempts to connect to internet: 
	// Ethernet being highest preference, then WiFi, then cellular. 

	// This way, PushingBox (or Adafruit IO for that matter) do not also need to have a communication platform specified, 
	// and just take the first available platform from a list of decreasing optimality.
#endif

#if is_ethernet == 1
	#include <Ethernet2.h>			// (this is needed for IPAddress, do not remove)
	
	//Use this for OPEnS Lab
	// byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6}; 

	byte mac[] = {0x00, 0x23, 0x12, 0x12, 0xCE, 0x7D};
	IPAddress ip(128,193,56,138); 
#endif

// --- Adafruit IO Options ---
#if is_adafruitio == 1
	#define AIO_USERNAME    "Goertzel"
	#define AIO_KEY         ""
#endif

// --- SD Options ---
#if is_sd
	// timestamp options:
	// 0: none, 1: date, 2: time, 3: date+time two fields, 4: data+time combined field
	#define sd_save_time_format 3
#endif

// --- RTC Options ---
#if is_rtc == 1
	// Select only one of the below options
	#define is_rtc3231 0 	// RTC DS 3231 Featherwing
	#define is_rtc8523 1	// RTC Adalogger Featherwing with PCF8523 RTC (the one with SD card)
#endif

// --- Delay between loops
#if is_lora == 0 						// Cannot not use with LoRa
	#define is_sleep_period 80			// Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. 
										// Change the value according to the length of your desired transmission interval
										// 80 seems to be a good amount, around 50 and lower may result in lost packets over WiFi
#else
	#define is_sleep_interrupt 11			// Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
#endif
