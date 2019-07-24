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
	Loom.nRF().set_print_verbosity(Verbosity::V_HIGH);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (Loom.nRF().receive()) {
		Loom.display_data();
		Loom.SDCARD().log("nrf.csv");
	}

	delay(100);

}
