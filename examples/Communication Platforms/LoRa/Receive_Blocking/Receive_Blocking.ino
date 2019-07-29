#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom("");


void setup() 
{ 
	Loom.begin_serial();
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (Loom.LoRa().receive_blocking(5000)) {
		Loom.print_internal_json();
		Loom.SDCARD().log("received.csv");
	}
}
