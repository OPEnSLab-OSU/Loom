///////////////////////////////////////////////////////////////////////////////

// This is a simple example demonstrating how to extract data as collected 
// by Loom in the measure(), package() sequence.

// The data is stored in a JsonObject which you can get directly using 
// 		JsonObject data = Exec.internal_json();

// You can also pass a JsonObject to Exec.package to be filled:
// 		JsonObject data;
// 		Exec.package(data);

// We recommend the usage shown below if you only need values individual keys.
// You need the module name and the data key to get the data.
// You should also know the type of the data, but you can cast it to a different 
// type if needed

// This is not a full example program and only collects data once.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Exec{};


void setup() 
{ 
	Exec.begin_LED();
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();


	Exec.measure();
	Exec.package();
	Exec.display_data();

	// Get analog A0 value
	LPrintln("A0 Val: ", Exec.get_data_as<int>("Analog", "A0") );
	// Get analog casting to float
	LPrintln("A0 Val: ", Exec.get_data_as<float>("Analog", "A0") );
	
	// Get battery value
	LPrintln("Vbat Val: ", Exec.get_data_as<float>("Analog", "Vbat") );
	
	// Get digital 6 value
	LPrintln("D6 Val: ", Exec.get_data_as<int>("Digital", "6") );

	// Try to get values that don't exist
	LPrintln("Unknown Val 1: ", Exec.get_data_as<int>("Analog", "Unknown") );
	LPrintln("Unknown Val 2: ", Exec.get_data_as<float>("Unknown", "Key") );
	LPrintln("Unknown Val 3: ", Exec.get_data_as<const char*>("AlsoUnknown", "Data") );
}

void loop() 
{
	// No loop code for this example
}
