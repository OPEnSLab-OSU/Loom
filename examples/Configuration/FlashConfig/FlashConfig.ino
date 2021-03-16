///////////////////////////////////////////////////////////////////////////////

// This example is used to demonstrate how to save a configuration to flash
// memory.
// Note: this funtionality is only useful if you are going to be updating 
// configurations at runtime and need the config to persist across startups

// This is a minimal example that increments the device instance number each
// restart to demonstrate persistence of config

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Disabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);

	// Read configuration
	// - Will read json_config (from config.h) on first run after flashing
	// - Will use flash configuration on subsequent startups until flashed again
	Loom.load_persistent_config(json_config);
	Loom.print_config(true);

	// Increment device instance number
	auto current_instance = Loom.get_instance_num();
	Loom.set_instance_num(current_instance+1);

	// Show that the json config is updated
	Loom.get_config(); // Queries all modules for current config
	LPrintln("\n\nUpdated config with incremented instance number");
	Loom.display_data(); // get_config() reads into the same Json document as data is built in	

	// Save this new config
	// NOTE: Do not call this function too frequently as the flash memory will degrade over time
	Loom.save_flash_config(); // Note that this will call get_config() again

	LPrintln("\n ** Setup Complete ** ");
	LPrintln("Restart Feather to see updated instance number");
}


void loop() 
{
	// No program here, example is for how to save config to flash
}
