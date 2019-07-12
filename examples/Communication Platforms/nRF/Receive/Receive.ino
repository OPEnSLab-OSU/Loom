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
	// Loom.LoRa().set_print_verbosity(Verbosity::V_HIGH);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (Loom.nRF().receive()) {
		Loom.print_internal_json();
		Loom.SDCARD().log("received.csv");
	}

	delay(100);

}
