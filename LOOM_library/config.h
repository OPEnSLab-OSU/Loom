// ================================================================ 
// ===                   CONFIGURATION FILE                     === 
// ================================================================
//
// Use this file to configure the device you are flashing the firmware of
//
// This file is divided into two sections:
//
//  Options: The top half that can be modified to change common settings
//
//  Advanced Options: the bottom section that is at least partially set based on  
//    the top half but may be modified for more specific behavior. 
//    Also contains other specific options that don't need to change as frequently
//
// Definitions that should not be modified by the user are
//   located in the library and are not present here

// ================================================================
// ================================================================ 
// ===                     PRIMARY OPTIONS                      === 
// ================================================================
// ================================================================



// ================================================================ 
// ===                  IDENTIFICATION OPTIONS                  === 
// ================================================================
// --- Device Identification --- 
#define FAMILY 		"LOOM"		// Will usually be "LOOM", you can change this if you are setting up your own network
#define FAMILY_NUM       5		// Specifies the subnet of the family that the device is on. 0 for elevated permissions (can communicate with any subnet), 1-9 for normal
#define CHANNEL          1		// Channel to use. Set to 1-8 for channels A-H, respectively (on WiFi), LoRa can use 1-9. Alternatively can define to -1 to used advanced option INIT_INST
#define REQUEST_SETTINGS 0		// 1 to request dynamic channel settings (i.e. next available channel) from MaxMSP Channel Manager, 0 to not
#define AUTO_NAME        1		// 1 to enable naming device based on configured settings (if not set manual name in advanced options)
#define CUSTOM_NAME "Custom"	// This is only used if Auto_name is set to be 0


// ================================================================ 
// ===                      SERIAL OPTIONS                      === 
// ================================================================
// --- Debugging --- 
#define LOOM_DEBUG    0			// Set to 1 if you want Serial statements from various functions to print
								// NOTE: Serial monitor must be opened for device to setup if this is enabled
								//   Device will freeze if this in enabled and device does not get plugged into Serial
								// LOOM_DEBUG_Print* are Serial prints that are removed if debugging is off

#define dynamic_serial_output 0 // These only apply if LOOM_DEBUG is enabled
								// 0 is standard operation 
								//   - Serial monitor needs to start / be open for device to setup
								//   - Serial (USB) can generally be detached after setup without issue 
								//   - Note that you may need to reopen the Serial monitor if it was open before the device was plugged back in
								// 1 Allows serial monitor to device to setup when LOOM_DEBUG is enabled without Serial (USB) being attached 
								//   - Note that you probably want to have the serial monitor open before uploading to the device, else you may miss 
								//     the first Serial prints, as enabling this option means that the device does not wait for you to open Serial

#define wake_delay 0			// 1 to enable wait 5 seconds upon awaking from sleep to (re)start Serial before program continue (useful for reprogramming if device quickly goes back to sleep)


#define prevent_platform_compile_error 1  	// 0: Allow errors to be triggered if the program calls 
											//     functionality of platforms / devices that are not enabled
											// 1: Permits the code to compile and the device will skip functionality
											//     of platforms / devices that are not enabled, informing the user of such

#define display_config 1		// 1 to print the uploaded configuration at start of setup, 0 to not

// ================================================================ 
// ===                  DEVICE TELEMETRY TYPE                   === 
// ================================================================
#define hub_node_type 1		// 0: Hub, 1: Node
#define is_repeater   0		// Sorry, this doesn't do anything yet


// ================================================================ 
// ===              PREDEFINED AGGREGATE DEVICES                === 
// ================================================================
// Enable specified devices, typically mutually exclusive
// Further options in the advanced settings
// Can override settings defined before the aggregate devices

#define is_ishield      1	// 1 to specify using Ishield (generally used on WiFi)
#define is_multiplexer  0	// 1 to specify Multiplexer (tca9548a) is being used
#define is_sapflow      0	// 1 to specify Sapflow  
#define is_evaporimeter 0	// 1 to specify Evaporimeter


// ================================================================ 
// ===                 COMMUNICATION PLATFORMS                  === 
// ================================================================
#define is_wifi       1		// 1 to enable WiFi
#define is_lora       0		// 1 to enable LoRa (cannot be used with nRF) (Further customization in advanced options)
#define is_nrf        0		// 1 to enable nRF (cannot be used with LoRa) (Further customization in advanced options)
#define is_ethernet   0		// 1 to enable Ethernet (a number of options below might auto enable this anyway though)
#define is_fona       0		// 1 to enable cellular via Fona (808 version)


// ================================================================ 
// ===                  DATA LOGGING PLATFORMS                  === 
// ================================================================
#define is_pushingbox 0		// 1 to enable PushingBox  
// #define is_adafruitio 0		// 1 to enable Adafruit IO (currently requires WiFi) [not much support yet]

// --- RTC Options ---
#define is_rtc        0		// Enable RTC functionality
#if is_rtc == 1
	#define RTC_pin 6		// What pin the RTC interrupt is connected to

	// Select only one of the below options
	#define is_rtc3231 0 	// RTC DS 3231 Featherwing
	#define is_rtc8523 1	// RTC Adalogger Featherwing with PCF8523 RTC (the one with SD card)
#endif

// --- SD Options ---
#define is_sd         0		// 1 to enable SD card 
#if is_sd == 1
	// Does NOT automatically save to SD
	// This works more like a filter than an automator,
	// with a minimum of sdSaveMinDelay between saves
	// Uses millis
	#define sdSaveFilter  	 1 	// 1 to enable a millis delay to SD 
	#define sdSaveMinDelay   1  // minimum delay in seconds between saves (only applies sdSaveFilter is 1)

	#define sdBroadcastSave  0  // 1 to broadcast on communication platforms when data is saved to SD, 0 to not 
#endif


// ================================================================ 
// ===                        ACTUATORS                         === 
// ================================================================

// --- Enabled Actuators --- 
#define num_servos    0 	// Number of servos being used (up to 8 per shield, testing has generally only been through 1 shield)
#define num_steppers  0		// Number of stepper motors being used 
#define is_relay      0		// 1 if a relay is being used
#if is_relay == 1
	#define relay_pin 10	// Digital pin connected to the relay enable 
#endif



// ================================================================ 
// ===                         SENSORS                          === 
// ================================================================

// --- Button Options ---
#define is_button 		1	// 1 to enable button
#define button_pin 		10	// Pin button is attached to 

// --- Analog Setting --- 
#define is_analog     0		// 1 if analog input is being used, 0 if not

	#define is_analog_a0 1 
	#define is_analog_a1 1
	#define is_analog_a2 0
	#define is_analog_a3 0
	#define is_analog_a4 0
	#define is_analog_a5 0

#define analog_samples 16 	// Must be 1, 2, 4, 8, or 16 number of analog measurements to sample and average



// --- Decagon Soil Moisture ---
#define is_decagon    0		// 1 if GS3 Decagon is being used

// --- I2C Sensors ---
// Using I2C sensors without the multiplexer
// Multiplexer / aggregate device may override these settings
#define is_tsl2591         0	// Lux Sensor
#define is_tsl2561         0	// Lux Sensor
	#if is_tsl2561 == 1
		#define tsl2561_res 3 // 1 for fastest, low-res, 2 for middle, 3 for slow, high-res
		// #define is_tsl2561_low   1 
		// #define is_tsl2561_float 1
		// #define is_tsl2561_high  1
	#endif
#define is_fxos8700        0	// Accelerometer / Magnetometer
#define is_fxas21002       0	// Gyroscope
#define is_zxgesturesensor 0	// ZX_Distance Sensor
#define is_sht31d          0	// Temperature / Humidity
#define is_mb1232          0	// Sonar
#define is_mpu6050         0	// Accelerometer / Gyroscope
#define is_lis3dh          0	// Accelerometer
#define is_ms5803          0	// Atmospheric Pressure / Temperature Sensor
#define is_hx711           0    // Load Cell
	#if is_hx711 == 1
		#define hx711_calibration 961.275
	#endif


// ================================================================ 
// ===                     WIFI SETTINGS                        === 
// ================================================================
// --- WiFi Settings ---
// Requires is_wifi to be set to 1 to take effect
#if is_wifi == 1
	#define DEFAULT_MODE      WPA_CLIENT_MODE	// AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
	#define DEFAULT_NETWORK   "OPEnS"			// Network SSID / name
	#define DEFAULT_PASSWORD  "arduino101"		// Network password
#endif




// ================================================================
// ================================================================ 
// ===                    ADVANCED OPTIONS                      === 
// ================================================================
// ================================================================

// --- Flash Options ---
#define enable_flash 1   			// 1 is the default, 0 to turn of reading and writing to flash/EEPROM memory 

// --- Scripts ---
#define enable_hub_scripts 0		// RPN script parser

// --- Advanced Interdev Communication ---
#define advanced_interdev_comm   0	// 1 to use Max-like functionality
#define build_actuator_cmd_funcs 0  // 1 to include file to make commands for Loom actuators

// --- MPU6050 Options
#define skip_mpu_cal 1


// --- Set Custom Instance Number and UDP Port ---
// If not using channels, then the following 
// intial instance number and port (if WiFi) will be used
// Interdevice communication is undefined and less supported
// when not using channels
#if CHANNEL == -1
	#define INIT_INST   	 3	// Initial device instance number (normally 1-9 when using channels)
	#if is_wifi == 1
		#define INIT_PORT 9443	// Initial device UDP port (normally 1-9 when using channels)
	#endif
#endif

#if is_wifi == 1
	#define advanced_interdev_comm 1 	// Enable for full Max support 
										// Default is for this to be 1 when using WiFi
#endif

#if LOOM_DEBUG == 1
	#define SERIAL_BAUD 115200 			// Select Serial baudrate, only used supported rates (i.e. 9600, or 115200, etc.)
										// Make sure the Serial monitor is set to the same baud
#endif




// ================================================================
// ================================================================
// ===            AGGREGATE DEVICE CONFIGURATIONS               === 
// ================================================================
// ================================================================

// These may override settings defined above 

// ================================================================
// ===                         ISHIELD                          === 
// ================================================================
#if is_ishield == 1
	#define is_button     1			// 1 to enable button
	#define button_pin    10		// Pin that the button uses
	#define is_mpu6050    1 		// Enables MPU6050 on Ishield
	#define is_neopixel   1			// Toggle based on whether Neopixels are being used 
	#define is_analog     1

	#define is_analog_a0 1			// 1 to enable Feather A_ ports as analog inputs 
	#define is_analog_a1 1
	#define is_analog_a2 0
	#define is_analog_a3 0
	#define is_analog_a4 0
	#define is_analog_a5 0

	#if is_neopixel == 1			// Which Ishield ports to enable Neopixels for 
		#define NEO_0     0			// Port 0 (A0, closest to end of Ishield)
		#define NEO_1     0			// Port 1 (A1, middle port)
		#define NEO_2     1			// Port 2 (A2, port closest to MPU6050)
	#endif  
#endif


// ================================================================
// ===                       MULTIPLEXER                        === 
// ================================================================
#if is_multiplexer == 1
	#define UPDATE_PERIOD 5000		// Milliseconds between multiplexer sensor list being updated
	
	// 1 to enable supported sensor type
	#define is_fxas21002       1	// Gyroscope
	#define is_fxos8700        1	// Accelerometer / Magnetometer
	#define is_lis3dh          1    // Accelerometer
	#define is_mb1232          1	// Sonar
	#define is_mpu6050         0	// Accelerometer / Gyroscope (NOTE* I2C address conflicts with RTC if not manually changed) (much better supported on Ishield)
	#define is_ms5803          1	// Pressure Sensor
	#define is_sht31d          1	// Temperature / Humidity
	#define is_tsl2561         1	// Lux Sensor
	#define is_tsl2591         1	// Lux Sensor
	#define is_zxgesturesensor 1	// ZX_Distance Sensor

	#define is_button 		   1	// 1 to enable button
#endif


// ================================================================
// ===                      SAPFLOW METER                       === 
// ================================================================

#if is_sapflow == 1
	// #define hub_node_type 0       // 0: hub, 1: node
	#define is_lora           1      // enable LoRa
	#define is_rtc            1
	#define is_sd             1


	#if hub_node_type == 0       // if is hub
		#define is_ethernet   1
		#define is_pushingbox 1
	#elif hub_node_type == 1     // if is node
		#define num_analog    2      // two temperature sensors
		#define probe_type    0      // 0:TDM, 1: HRM
		#define is_sht31d     1      // Temperature / Humidity
		#define is_decagon    1      // Soil moisture
		#define heatpulse     2500   // For HRM probe heat pulse (e.g 2500:2.5 sec) 
		#define senddelay     60000  // send data every 1 min
		#define heatduration  30000  // Turn on heater for 30 sec
	#endif
#endif 


// ================================================================
// ===                       EVAPOROMETER                       === 
// ================================================================
#if is_evaporimeter == 1
	#define hub_node_type 1			// Don't change, as all Evaporimeters are nodes, hubs are not Evaporometers
	#define is_lora       1		

	// Lux Sensor
	#define is_tsl2561 1
		#define is_tsl2561_low   1 	// Generally the upper light sensor
		#define is_tsl2561_float 1  // Generally the lower light sensor
		#define is_tsl2561_high  0  

	// Load Cell
	#define is_hx711 1
	#if is_hx711 == 1
		#define hx711_calibration 961.275 // The weight of the bucket with wick (grams)
	#endif

	// Temp / Humidity
	#define is_sht31d 1

	// RTC
	#define is_rtc 1
	#if is_rtc == 1
		#define RTC_pin    6
		#define is_rtc3231 1
		#define is_rtc8523 0
	#endif

	#define is_button  0  // To prevent data from non-existent button to be sent
#endif


// ================================================================ 
// ===             COMMUNICATION PLATFORM OPTIONS               === 
// ================================================================

// --- LoRa Options ---
#if is_lora == 1

	// Address of hub / node
	// Self and corresponding device 
	// 10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS,
	// 10-19 for 0, 20 - 29 for 1, etc. 
	#if hub_node_type == 0 	// If is hub
		// #define LORA_HUB_ADDRESS  CHANNEL
		// #define LORA_NODE_ADDRESS 1
	#else 					// If is node
		#define LORA_HUB_ADDRESS  1			
		// #define LORA_NODE_ADDRESS CHANNEL
	#endif
										
	#define RF95_FREQ      915.0			// Hardware specific, Tx must match Rx

	// #define lora_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send
											// Currently unused
#endif

// --- nRF Options --- 
#if is_nrf == 1
	#if hub_node_type == 0 	// If is hub
		// #define NRF_HUB_ADDRESS  CHANNEL
		// #define NRF_NODE_ADDRESS 1
	#else 					// If is node
		#define NRF_HUB_ADDRESS  1			
		// #define NRF_NODE_ADDRESS CHANNEL
	#endif

	// #define nrf_bundle_fragment 0		// Splits bundles into smaller bundles to avoid overflowing size LoRa can send
											// Currently unused
#endif

// --- Ethernet Options ---
#if is_ethernet == 1
	#include <Ethernet2.h>				// (this is needed for IPAddress object below, do not remove)
	
	//Use this for OPEnS Lab
	byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6}; 

	// byte mac[] = {0x00, 0x23, 0x12, 0x12, 0xCE, 0x7D};    // MAC address of Ethernet Luke's port
	
	IPAddress ip(128,193,56,138); 						  // device's IP address  				try to make this a string, so #include can be move to ethernet file
#endif


// --- FONA Options ---
#if is_fona == 1
	#define LOOM_DEBUG 1	// This is just temporary for now, as all Fona tests are done interactively through serial monitor
	#define fona_test  1    // This enables the Fona interactive testing Loop, note that this will override normal 
							//    Loom loop behavior until command to exit testing loop is issued
	#define fona_type 2G 	// Can be '2G' (808) or '3G' (5320A), currently only 2G is supported
#endif


// ================================================================ 
// ===                       HUB OPTIONS                        === 
// ================================================================

#if (hub_node_type == 0) && ((is_wifi == 1) || (is_ethernet == 1) || (is_fona == 1))
	#define is_pushingbox 1     	// Auto enable PushingBox if Ethernet, WiFi, or cellular

	#define device_poll_refresh 0 	// Seconds between re-searching for devices on the network
									// Set 0 or negative to disable
#endif


// ================================================================ 
// ===                 DATA LOGGING OPTIONS                     === 
// ================================================================

// --- PushingBox Options ---
#if (is_ethernet != 1) && (is_wifi != 1) && (is_fona != 1)
	#define is_pushingbox 0   // Prevent PushingBox if no means of internet access
#endif
#if is_pushingbox == 1	
	// Google Spreadsheet ID
	// (found betweeen the "docs.google.com/spreadsheets/d/" and "/edit..." in the URL; random string of characters)

	// #define spreadsheet_id "17XjrTjXENChYsHMVvKgePg8rsohwz0hyASkdeZZKROk"
	// #define spreadsheet_id "16K7gOczeewt-wVHdnMR0ttWSrcqmVvWvG-2zJxo1-MA"	    
	#define spreadsheet_id "1Hv2oME5sjumUXv36GtFV1Q7I83xnXu-f-ZrxUNsXS_U"  // This is Luke's Arduino Test spreadsheet

	// Required by PushingBox, specific to each scenario
	char device_id[] = "v7ECCEF7A460E57A";

	// Google Spreadsheet Sheet/Tab. Sent as parameter to PushingBox/Google Scripts
	// Generally 0 is the preferred setting (i.e. nodes get there own tab),
	//   use 1 to force all node data to go to a single hub-defined sheet
	#define useHubTabID 0  // Toggle option for below settings
	#if useHubTabID == 1
		// The hub defines tab ID regardless of bundle source
		#define tab_id_complete "Sheet1"    // Defines tab if hub is defining tab instead of node
	#else
		// Use bundle source and below prefix to define tab ID
		#define tab_id_prefix   "E_"		// Used as a prefix if node is being used to define tab
	#endif	

	#define verify_family_match 1			// 1 to only upload to spreadsheet if source device family matches hub 


	// Currently works by only sending a bundle from 
	// log_bundle/sendToPushingBox if the delay has passed
	// Does NOT automatically upload a bundle, you still have to call sendToPushingBox
	// This works more like a filter than an automator,
	// with a minimum of pushUploadMinDelay between uploads
	// Uses millis
	// Recommended that pushUploadedFilter is left enabled
	#define pushUploadFilter   	1 	// 1 to enable a millis delay to uploading to PushingBox
	#define pushUploadMinDelay  5  	// delay in seconds
#endif


// --- Adafruit IO Options ---
#if is_adafruitio == 1
	#define AIO_USERNAME    ""
	#define AIO_KEY         ""
#endif


// --- SD Options ---
#if is_sd
	// Timestamp formatting options:
	// 0: none, 1: date, 2: time, 3: date+time two fields, 4: data+time combined field
	// Times are only added if RTC is also enabled
	#define sd_save_time_format 3
#endif


// ================================================================
// ===              BETWEEN LOOP SLEEP OPTIONS                  === 
// ================================================================

// --- Delay between loops ---
#if is_lora == 0 						// Cannot not use with LoRa
	#define is_sleep_period 50			// Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. 
										// Change the value according to the length of your desired transmission interval
										// 80 seems to be a good amount, around 50 and lower may result in lost packets over WiFi
#else
	#define is_sleep_interrupt 11			// Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
#endif




// ================================================================
// ===               I2C Sensor Address Selection               === 
// ================================================================

// 1 to enable a given I2C address, 0 to disable
// Multiplexers shouldn't usually have to modify the enabled addresses section unless
//   Multiple of the same sensors are being used and have different I2C addresses

// You can also name your sensors if not using a multiplexer, which
//	can be useful for distinquishing sensors 
//  names will not be used when the sensor is connected via a multiplexer

// Make sure you have no conflicts of addresses 
//	(e.g. TSL2561 and TSL2591 both on 0x29)

// You can have multiple of the same sensor

// If there is only one I2C address available, the enable/disable feature is still here
//  for completeness sake - if you don't use a given sensor disable it with the is_<sensor> define instead though





#if is_fxas21002 == 1
	// #define i2c_addr_fxas21002_0x20 1 
	#define i2c_addr_fxas21002_0x21	1 // Default

	#if is_multiplexer != 1
		// #if i2c_addr_fxas21002_0x20 == 1
		// 	#define fxas21002_0x20_name "FXAS21002_0x20"
		// #endif 
		#if i2c_addr_fxas21002_0x21 == 1
			#define fxas21002_0x21_name "FXAS21002_0x21"
		#endif 
	#endif
#endif

#if is_fxos8700 == 1
	// #define i2c_addr_fxos8700_0x1C 	0 
	// #define i2c_addr_fxos8700_0x1D 	0
	// #define i2c_addr_fxos8700_0x1E 	0
	#define i2c_addr_fxos8700_0x1F 	1 // Default

	#if is_multiplexer != 1
		// #if i2c_addr_fxos8700_0x1C == 1
		// 	#define fxos8700_0x1C_name "FXOS8700_0x1C"
		// #endif 
		// #if i2c_addr_fxos8700_0x1D == 1
		// 	#define fxos8700_0x1D_name "FXOS8700_0x1D"
		// #endif 
		// #if i2c_addr_fxos8700_0x1E == 1
		// 	#define fxos8700_0x1E_name "FXOS8700_0x1E"
		// #endif 
		#if i2c_addr_fxos8700_0x1F == 1
			#define fxos8700_0x1F_name "FXOS8700_0x1F"
		#endif 
	#endif
#endif

#if is_lis3dh == 1
	#define i2c_addr_lis3dh_0x19 	1

	#if is_multiplexer != 1
		#if i2c_addr_lis3dh_0x19 == 1
			#define lis3dh_0x19_name "LIS3DH_0x19"
		#endif 
	#endif
#endif

#if is_mb1232 == 1
	#define i2c_addr_mb1232_0x70	1

	#if is_multiplexer != 1
		#if i2c_addr_mb1232_0x70 == 1
			#define mb1232_0x70_name "MB1232_0x70"
		#endif 
	#endif
#endif


// This may not work as intended yet - may need to modify 
// i2c_addr_mpu6050 at top of loom_mpu6050.h
#if is_mpu6050 == 1
	#define i2c_addr_mpu6050_0x68 	1 // Default
	#define i2c_addr_mpu6050_0x69	0

	#if is_multiplexer != 1
		#if i2c_addr_mpu6050_0x68 == 1
			#define mpu6050_0x68_name "MPU6050_0x68"
		#endif 
		#if i2c_addr_mpu6050_0x69 == 1
			#define mpu6050_0x69_name "MPU6050_0x69"
		#endif 
	#endif
#endif

#if is_ms5803 == 1
	#define i2c_addr_ms5803_0x76 	1 // Default
	// #define i2c_addr_ms5803_0x77	0

	#if is_multiplexer != 1
		#if i2c_addr_ms5803_0x76 == 1
			#define ms5803_0x76_name "MS5803_0x76"
		#endif 
		// #if i2c_addr_ms5803_0x77 == 1
		// 	#define ms5803_0x77_name "MS5803_0x77"
		// #endif 
	#endif
#endif

#if is_sht31d == 1
	#define i2c_addr_sht31d_0x44 	1 // Default
	#define i2c_addr_sht31d_0x45	0

	#if is_multiplexer != 1
		#if i2c_addr_sht31d_0x44 == 1
			#define sht31d_0x44_name "SHT31D_0x44"
		#endif 
		#if i2c_addr_sht31d_0x45 == 1
			#define sht31d_0x45_name "SHT31D_0x45"
		#endif 
	#endif
#endif

#if is_tsl2561 == 1
	// 2 are used on evaporometer b/c no multiplexer
	#define i2c_addr_tsl2561_0x29 	0 // generally disabled because 2591 uses this address, disable that to use 2561 on the address
	#define i2c_addr_tsl2561_0x39 	1 
	#define i2c_addr_tsl2561_0x49 	1
	
	#if is_multiplexer != 1
		#if i2c_addr_tsl2561_0x29 == 1
			#define tsl2561_0x29_name "TSL2561_0x29"
		#endif 
		#if i2c_addr_tsl2561_0x39 == 1
			#define tsl2561_0x39_name "TSL2561_0x39"
		#endif 
		#if i2c_addr_tsl2561_0x49 == 1
			#define tsl2561_0x49_name "TSL2561_0x49"
		#endif 
	#endif
#endif

#if is_tsl2591 == 1
	#define i2c_addr_tsl2591_0x29 	1

	#if is_multiplexer != 1
		#if i2c_addr_tsl2591_0x29 == 1
			#define tsl2591_0x29_name "TSL2591"
		#endif 
	#endif
#endif

#if is_zxgesturesensor == 1
	#define i2c_addr_zxgesturesensor_0x10 	1 // Default
	#define i2c_addr_zxgesturesensor_0x11	0

	#if is_multiplexer != 1
		#if i2c_addr_zxgesturesensor_0x10 == 1
			#define zxgesturesensor_0x10_name "ZXGESTURESENSOR_0x10"
		#endif 
		#if i2c_addr_zxgesturesensor_0x11 == 1
			#define zxgesturesensor_0x11_name "ZXGESTURESENSOR_0x11"
		#endif 
	#endif
#endif



