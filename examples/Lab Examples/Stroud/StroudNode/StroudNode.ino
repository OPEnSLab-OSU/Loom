///////////////////////////////////////////////////////////////////////////////

// This example is the code used for the OPEnS Lab's Stroud workshop.

// The device expects to have:
// - Feather M0 WiFi
// - OPEnS Lab hydro board (not necessary if you wire manually and don't need 
//		to turn sensors off)
// - Adafruit Adalogger Featherwing
// - MS5803 pressure sensor
// - Analog sensors:
// 		- EC / Total dissolved solids (pin 0)
// 		- pH (pin A1)
// 		- Turbidity (pin A2)

// The device has two modes:
// - Max realtime visualization and interactivity over WiFi
//		- Collects data
//		- Log to SD and sends data to Max
//		- Can receive commands from Max (such as actuation or loading a 
//			different configuration from SD)
// - Google Sheets logging over WiFi
//		- Collects data
//		- Log to SD
//		- Sends data to Google sheets
// The mode is determined at startup by reading a switch connected to pin 9
// - Max if switch is LOW
// - Google sheets is switch is HIGH

// The device makes use of the SD configurations, selecting its configuration 
// based on the state of the switch.
// If it fails to load the configuration from SD, it will default to the configuration
// in the config.h file, which is for Max operation 

// The provided configurations should be copied to a micro SD card to be used on the device.
// They will not work as provided, you must fill out/configure the following as needed:
// - Device name 
// - Instance number
// - Interval
// - SD default save file
// - WiFi SSID
// - WiFi Password
// - Google script
// - Googlesheet ID
// - Googlesheet Tab Name

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::WiFi,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	pinMode(9, INPUT_PULLUP);			// To detect mode
	// pinMode(10, OUTPUT);				// To be able to turn sensor on/off 
										// Not currently being used due to conflict with SD CS pin

	Loom.begin_LED();					// LED setup
	Loom.begin_serial(false);			// Start Serial, false indicates don't wait for Serial Monitor

	// Try to load SD config based on mode
	bool use_max = digitalRead(9) == 0; 		// High to use Max, Low to use Google Sheets
	LPrintln("In ", use_max ? "Max" : "GoogleSheets", " mode");

	bool load_success = (use_max) 
				? Loom.parse_config_SD("Max.txt") 
				: Loom.parse_config_SD("Google.txt");

	// If SD config failed, use #include'd config from above
	if ( !load_success ) {
		use_max = true; // Fallback configuration provided is for Max use
		Loom.parse_config(json_config);
	}

	Loom.print_config(true);				// Print config

	LPrintln("\n ** Setup Complete ** ");

	Loom.flash_LED(5, 50, 50, false);		// Flash to indicate is setup
}


void loop() 
{
	// digitalWrite(10, HIGH);	// Turn on sensors
	// Loom.pause(100);			// Warmup time
	Loom.measure();				// Collect data
	// digitalWrite(10, LOW);	// Turn off sensors

	Loom.package();				// Build Json from data
	Loom.display_data();		// Print data (will not work properly if using nap instead of pause below)

	Loom.SDCARD().log();		// Log to SD

	// The following if statements are a means of having both Max and GoogleSheets modes
	// work with the same code. If a module exists, it will use it, otherwise it was 
	// probably not instantiated for this mode and will skip it.

	if (Loom.has_module(LoomModule::Type::MaxPub)) {
		Loom.MaxPub().publish();					// Send data to Max
	}
	if (Loom.has_module(LoomModule::Type::MaxSub)) {
		Loom.MaxSub().subscribe();					// Receive any messages from Max
	} 
	if ( Loom.has_module(LoomModule::Type::GoogleSheets) ) {
		Loom.GoogleSheets().publish();				// Send data to Google Sheets
	}

	// Loom.pause(); 				// Wait (delay) based on 'interval' value in config
	Loom.nap(); 				// Wait (sleepy dog) based on 'interval' value in config
}



