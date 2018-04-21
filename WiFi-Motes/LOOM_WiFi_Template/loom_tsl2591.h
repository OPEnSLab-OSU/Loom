//Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

//Structures 
struct config_tsl2591_t {

};

struct state_tsl2591_t {
	Adafruit_TSL2591 inst_tsl2591;
	
};

//Global Declarations
struct config_tsl2591_t *config_tsl2591;
struct state_tsl2591_t *state_tsl2591;


//Functions
void setup_tsl2591() {
	
}

void package_data_tsl2591(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_tsl2591() {
	
}

