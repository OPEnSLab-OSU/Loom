// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <ZX_Sensor.h>

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_zxgesturesensor_t {

};

struct state_zxgesturesensor_t {

};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_zxgesturesensor_t *config_zxgesturesensor;
struct state_zxgesturesensor_t *state_zxgesturesensor;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_zxgesturesensor();
void package_data_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]);
void measure_zxgesturesensor();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
void setup_zxgesturesensor() {
  //Setup Here
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_zxgesturesensor() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

