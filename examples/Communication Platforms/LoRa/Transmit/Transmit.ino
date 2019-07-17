#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom{};


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

	// Send to address 1
	Loom.LoRa().send(1);

	delay(2000);
}

