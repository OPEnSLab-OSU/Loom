#include <Loom.h>

// Include configuration
// Optional if reading config from SD
// The following is what can be used if SD config not found
const char* json_config = 
#include "config.h"
;

LoomManager Loom("");


void setup() 
{ 
	Loom.begin_serial(true);

	// Get config from SD
	// Else use '#include'd json_config above
	if ( !Loom.use_SD_config("config4.txt") ) {
		Loom.parse_config(json_config);
	}
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	// Loom.measure();
	// Loom.package();
	// Loom.print_internal_json();

	// delay(2000);
}
