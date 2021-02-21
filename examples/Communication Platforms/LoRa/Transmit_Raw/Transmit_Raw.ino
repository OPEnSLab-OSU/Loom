///////////////////////////////////////////////////////////////////////////////

// Compare to the Transmit LoRa example, this example shows how to send  
// raw data using LoRa to the other board.

// The limit size of sending and receiving an individual package is 
// 255 bit over LoRa.
// Which means that if the package size is too big, then it won't be
// able to send the package to the other board. 

// Therefore, if someone wants to send larger package size over LoRa,
// this example will show how to do it. 

// Unlike other examples, this example need to modify 4 files.
// Please take a look of them carefully.

// This example was created by this GitHub Issue: 
// https://github.com/OPEnSLab-OSU/Loom/issues/69

// If you are looking at this example and want to use it, 
// then I would also recommend to look at Receive_Blocking_Raw example. 

// There will be a lot of coding in C++, so please follow the comments guide.
// If you are confused how to use it, then please send a GitHub Issue that 
// Loom team will be happy to support your code. 

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

#include "LoomJSON.h"

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup() 
{ 
	Feather.begin_serial(true);
	Feather.parse_config(LCONFIG);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

    // If you change the name from LoomJSON.h file, then change Loom_Base to the name that you changed. 
	Loom_Base out_struct;
    
    const JsonObjectConst internal_data = Feather.internal_json(false);
	Feather.get<Loom::LoRa>()->send_raw(out_struct.raw, sizeof(out_struct.raw), 1); // This raw data will be send to board id 1

	Feather.pause();	// Delay between interations set with 'interval' in config
}

