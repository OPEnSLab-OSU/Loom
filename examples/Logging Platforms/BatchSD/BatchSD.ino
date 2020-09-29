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

// In Tools menu, set:
// Internet  > LTE // To save storage, use 'Disabled' for any sections that are not included in the config file
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Exec{};

// Sets the maximum number amount of packets to be transmitted with the batch
const int max_packets = 10;
int counter;

void setup()
{
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	LPrintln("\n ** Setup Complete ** ");

	counter = 0;
}


void loop()
{
	Exec.measure();
	Exec.package();
	Exec.display_data();

  // This will log data to both SD (.csv file) and to the batch to be published/transmitted later
	Exec.log_all();
	counter++;

	// For every ten packets in the batch, publish using GoogleSheets
	if(counter%max_packets == 0) {
		Exec.get<Loom::GoogleSheets>().publish_batch();
		// Send to address 1, the second parameter defines the delay between sends and the default is 0. This applies for the functions down belows
		// With Transmitting with LoRa, if the hub receiving is publishing immediately to GoogleSheets, LoRa will need at least 4000ms of delay
		// Depending on how the hub process the packets receieved, it may need more or less time than 4000ms, so feel free to tinker with the timing
		//Exec.LoRa().send_batch(1, 4000);
		//Exec.Bluetooth().send_batch(1, 4000);
		//Exec.nRF().send_batch(1, 4000);
		//Exec.nRF().broadcast_batch(4000);
	}

	Exec.pause();
}
