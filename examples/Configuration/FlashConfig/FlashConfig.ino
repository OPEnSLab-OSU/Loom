///////////////////////////////////////////////////////////////////////////////

// This example is used to demonstrate how to save a configuration to flash
// memory.
// Note: this funtionality is only useful if you are going to be updating 
// configurations at runtime and need the config to persist across startups

// This is a minimal example that increments the device instance number each
// restart to demonstrate persistence of config

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet    > Disabled
// Sensors     > Disabled
// Radios      > Disabled
// Actuators   > Disabled
// Max         > Disabled
// FlashConfig > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup() 
{
	Feather.begin_LED();
	Feather.begin_serial(true);

	// Read configuration
	// - Will read LCONFIG (from config.yaml) on first run after flashing
	// - Will use flash configuration on subsequent startups until flashed
	Feather.load_persistent_config();
	Feather.print_config(true);

	// Increment device instance number
	auto current_instance = Feather.get_instance_num();
	Feather.set_instance_num(current_instance+1);

	// Show that the json config is updated
	Feather.get_config(); // Queries all modules for current config
	LPrintln("\n\nUpdated Config");
	Feather.display_data(); // get_config() reads into the same Json document as data is built in	

	// Save this new config
	// NOTE: Do not call this function too frequently as the flash memory will degrade over time
	Feather.save_flash_config(); // Note that this will call get_config() again

	LPrintln("\n ** Setup Complete ** ");
	LPrintln("Restart Feather to see updated instance number");
}


void loop() 
{
	// No program here, example is for how to save config to flash
}


