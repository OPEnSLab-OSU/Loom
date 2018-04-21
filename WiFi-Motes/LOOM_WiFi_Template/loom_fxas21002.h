// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_fxas21002_t {

};

struct state_fxas21002_t {
	Adafruit_FXAS21002C inst_fxas21002;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_fxas21002_t *config_fxas21002;
struct state_fxas21002_t *state_fxas21002;



// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
void setup_fxas21002() {
  //Setup Here
}

// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_fxas21002(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_fxas21002() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

