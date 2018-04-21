//Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>

//Structures 
struct config_fxos8700_t {

};

struct state_fxos8700_t {

};

//Global Declarations
struct config_fxos8700_t *config_fxos8700;
struct state_fxos8700_t *state_fxos8700;


//Functions
void setup_fxos8700() {
	//Setup Here
}

void package_data_fxos8700(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_fxos8700() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

