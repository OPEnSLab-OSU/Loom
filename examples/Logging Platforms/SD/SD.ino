#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom("");


void setup() 
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.print_internal_json();
	
	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	Loom.SDCARD().log();

	// Or log to a specific file (does not change what default file is set to)	
	// Loom.SDCARD().log("specific.csv");

	delay(5000);	
}
