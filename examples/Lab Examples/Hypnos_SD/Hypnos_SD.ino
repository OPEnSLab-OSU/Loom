///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates usage of the Hypnos board.

// The Hypnos board includes
// - SD
// - DS3231 RTC
// - Ability to power of peripherals

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

#define HYPNOS3 5
#define HYPNOS5 6

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

void setup() 
{
	// Needs to be done for Hypno Board
	pinMode(HYPNOS3, OUTPUT);		// Enable control of 3.3V rail 
	pinMode(HYPNOS5, OUTPUT);		// Enable control of 5V rail 

	//See Above
	digitalWrite(HYPNOS3, LOW);	// Enable 3.3V rail
	digitalWrite(HYPNOS5, HIGH);	// Enable 5V rail

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	Loom.SDCARD().log();

	Loom.pause();	
}
