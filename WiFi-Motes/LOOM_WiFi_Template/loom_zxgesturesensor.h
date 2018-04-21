//Libraries
#include <Wire.h>
#include <ZX_Sensor.h>

//Structures 
struct config_zxgesturesensor_t {

};

struct state_zxgesturesensor_t {

};

//Global Declarations
struct config_zxgesturesensor_t *config_zxgesturesensor;
struct state_zxgesturesensor_t *state_zxgesturesensor;


//Functions
void setup_zxgesturesensor() {
	//Setup Here
}

void package_data_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_zxgesturesensor() {
	//Measure data and change the state here. 
	//Potentially uses the config data
}

