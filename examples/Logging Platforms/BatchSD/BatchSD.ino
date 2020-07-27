///////////////////////////////////////////////////////////////////////////////

// This is a example that demonstrates how to log data to an SD card
// using a batch to be published or transmitted through the publishing or
// communication platforms later. This sketch also logs data to an SD card
// with a .csv file.
//
// For this sketch, you will need to remove the items not used from the config
// file. nRF, Bluetooth, LoRa and GoogleSheets with Ethernet are all options listed
// in the config file to transmit/publish the batch.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Ethernet,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

// Sets the maximum number amount of packets to be transmitted with the batch
const int max_packets = 10;
int counter;

void setup()
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");

  counter = 0;
}


void loop()
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

  // This will log data to both SD (.csv file) and to the batch to be published/transmitted later
	Loom.log_all();
  counter++;

  if(counter%max_packets == 0){
    Loom.GoogleSheets().publish_batch();
    // Send to address 1, the second parameter defines the delay between sends and the default is 0. This applies for the functions down belows
    //Loom.LoRa().send_batch(1, 0);
    //Loom.Bluetooth().send_batch(1);
    //Loom.nRF().send_batch(1);
    //Loom.nRF().broadcast_batch();
  }


	Loom.pause();
}
