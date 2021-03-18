///////////////////////////////////////////////////////////////////////////////

// An example of running Loom starting in access point WiFi mode. 
// Use the Max 'WiFi Setup' patch to command a Wifi mode change to save the 
// config (i.e. command to client mode).
// Subsequent starts will use the saved config (e.g. client mode WiFi on 
// startup until the device is reflashed.

///////////////////////////////////////////////////////////////////////////////

#include <LoomForMax.h>

// In config, instance number is number to set ModuleCore to in Max MSP

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
	Loom.begin_LED();
	Loom.begin_serial(false);

	// Read configuration
	// - Will read json_config (from config.h) on startup unless Manager.save_flash_config() is called
	// - save_flash_config() is called WiFi modes switch (as commanded from Max)
	// - Will use flash configuration on subsequent startups until flashed again
	Loom.load_persistent_config(json_config);
	Loom.print_config(true);
	
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();
	Loom.MaxPub().publish();
	Loom.MaxSub().subscribe();
	Loom.pause();
}
