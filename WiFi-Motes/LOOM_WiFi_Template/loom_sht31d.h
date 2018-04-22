// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "Adafruit_SHT31.h"

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_sht31d_t {

};

struct state_sht31d_t {

};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_sht31d_t *config_sht31d;
struct state_sht31d_t *state_sht31d;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void package_data_sht31d(OSCBundle *bndl, char packet_header_string[]);
void measure_sht31d();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
void setup_sht31d() {
  //Setup Here
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


void package_data_sht31d(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_sht31d() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

