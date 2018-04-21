// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_tca9548a_t {

};

struct state_tca9548a_t {

};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_tca9548a_t *config_tca9548a;
struct state_tca9548a_t *state_tca9548a;


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
void setup_tca9548a() {
  //Setup Here
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


void package_data_tca9548a(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_tca9548a() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

