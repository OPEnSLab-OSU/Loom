// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
// #include <ExampleLibrary.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_<module>_t {
	// int  stored_int;
	// char stored_string[32];
};

struct state_<module>_t {
	// float some_value;
	// int   array_example[3];

};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_<module>_t config_<module>;
struct state_<module>_t  state_<module>;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_<module>();
void package_<module>(OSCBundle *, char[]);
void measure_<module>();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for <module> that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
bool setup_<module>() 
{
	//Setup Here


	// Example of what setup might look like:

	// bool is_setup;
	// state_<module>.float = 1.2345;
	
	// if(<some setup condition>) {
	// 	is_setup = true;
	// 	#if LOOM_DEBUG == 1
	// 		Serial.println("Setup <module> successful.");
	// 	#endif
	// }
	// else {
	// 	is_setup = false;
	// 	#if LOOM_DEBUG == 1
	// 		Serial.println("Failed to setup <module>.");
	// 	#endif
	// }
	// return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE <MODULE> --- (Multiplexer Version)
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
// [@param port                  Which port of the multiplexer the device is plugged into]
//
void package_<module>(OSCBundle *bndl, char packet_header_string[], int port) 
{
	// Create a message and fill it here, then add it to the bndl

	// Example of what measure might look like:

	// char address_string[255];
	// sprintf(address_string, "%s%s", packet_header_string, "/<module>");
	// OSCMessage msg = OSCMessage(address_string);
	// msg.add("ex1").add(state_<module>.array_example[0]);
	// msg.add("ex2").add(state_<module>.array_example[1]);
	// msg.add("ex3").add(state_<module>.array_example[2]);
	// bndl->add(msg);
}

void package_<module>(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s", packet_header_string, "/module_example0");
	bndl->add(address_string).add(state_<module>.array_example[0]);
	sprintf(address_string, "%s%s", packet_header_string, "/module_example1");
	bndl->add(address_string ).add(state_<module>.array_example[1]);
}



// --- MEASURE <MODULE> ---
//
// Gets the current sensor readings of the <Module and stores into variables
// If debug prints are enabled, can also print any relevant information 
//
void measure_<module>() 
{
	// Measure data and change the state here. 
	// Potentially uses the config data

	// Example of what measure might look like:

	// state_<module>.some_value = ReadSensorValue
	// for (int i = 0; i < 3) {
	// 		state_<module>.array_example[i] = ReadOtherValue
	// }
}





