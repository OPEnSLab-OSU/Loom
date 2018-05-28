// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_<module>_t {

};

struct state_<module>_t {

};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_<module>_t *config_<module>;
struct state_<module>_t *state_<module>;


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
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE <MODULE> ---
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
// [@param port                  Which port of the multiplexer the device is plugged into]
//
void package_<module>(OSCBundle *bndl, char packet_header_string[]) 
{
	//Create a message and fill it here, then add it to the bndl
}



// --- MEASURE <MODULE> ---
//
// Gets the current sensor readings of the <Module and stores into variables
// If debug prints are enabled, can also print any relevant information 
//
void measure_<module>() 
{
	//Measure data and change the state here. 
	//Potentially uses the config data
}

