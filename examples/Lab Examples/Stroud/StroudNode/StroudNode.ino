#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;


LoomManager Loom{};



bool use_max; 


void setup() 
{ 
	pinMode(9, INPUT_PULLUP);			// To detect mode
	// pinMode(10, OUTPUT);				// To be able to turn sensor on/off 
										// Not currently being used due to conflict with SD CS pin

	Loom.begin_LED();					// LED setup
	Loom.begin_serial(false);			// Start Serial, false indicates don't wait for Serial Monitor

	// Try to load SD config based on mode
	use_max = digitalRead(9); 		// High to use Max, Low to use Google Sheets
	LPrintln("In ", use_max ? "Max" : "GoogleSheets", " mode");

	bool status = (use_max) 
				? Loom.parse_config_SD("Max.txt") 
				: Loom.parse_config_SD("Google.txt");

	// If SD config failed, use #include'd config from above
	if ( !status ) {
		use_max = true; // Fallback configuration provided is for Max use
		Loom.parse_config(json_config);
	}

	// if (use_max) {		// For extra print info
	// 	Loom.MaxPub().set_print_verbosity(Verbosity::V_HIGH);
	// 	Loom.MaxSub().set_print_verbosity(Verbosity::V_HIGH);
	// }

	Loom.print_config(true);				// Print config

	LPrintln("\n ** Setup Complete ** ");

	Loom.flash_LED(5, 50, 50, false);		// Flash to indicate is setup

}


int i = 0;

void loop() 
{
	// digitalWrite(10, HIGH);	// Turn on sensors
	// Loom.pause(100);			// Warmup time
	Loom.measure();				// Collect data
	// digitalWrite(10, LOW);	// Turn off sensors

	Loom.package();				// Build Json from data
	Loom.display_data();		// Print data (will not work properly if using nap instead of pause below)
	Loom.add_data("Packet", "Number", i++);		// Add packet number (not necessary)

	if (Loom.has_module(LoomModule::Type::MaxPub)) {
		Loom.MaxPub().publish();					// Send data to Max
	}
	if (Loom.has_module(LoomModule::Type::MaxSub)) {
		Loom.MaxSub().subscribe();					// Receive any messages from Max
	} 
	if ( Loom.has_module(LoomModule::Type::GoogleSheets) ) {
		Loom.GoogleSheets().publish();
	}

	// Loom.pause(); 				// Wait (delay) based on 'interval' value in config
	Loom.nap(); 				// Wait (sleepy dog) based on 'interval' value in config
}





