// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "Adafruit_SHT31.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_sht31d 0x44													 //0x44, 0x45


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_sht31d_t {

};

struct state_sht31d_t {
	Adafruit_SHT31 inst_sht31d;
	float temp;
	float humid;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_sht31d_t config_sht31d;
struct state_sht31d_t state_sht31d;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_sht31d();
void package_data_sht31d(OSCBundle *bndl, char packet_header_string[]);
void measure_sht31d();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_sht31d() {
  bool is_setup;
	if(state_sht31d.inst_sht31d.begin(i2c_addr_sht31d)) {
		is_setup = true;
		#if DEBUG == 1
			Serial.println("Initialized sht31d (temp/humid)");
		#endif
	}
	else {
		is_setup = false;
		#if DEBUG == 1
			Serial.println("Failed to initialize sht31d (temp/humid");
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


void package_data_sht31d(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_sht31d() {
	float t = state_sht31d.inst_sht31d.readTemperature();
  float h = state_sht31d.inst_sht31d.readHumidity();

	if((!isnan(t)) && (!isnan(h))) {
		state_sht31d.temp = t;
		state_sht31d.humid = h;
		#if DEBUG == 1
			Serial.print("Temp: ");
			Serial.println(state_sht31d.temp);
			Serial.print("Humidity: ");
			Serial.println(state_sht31d.humid);
		#endif
	}
	else {
		#if DEBUG == 1
			Serial.println("Failed to read temperature or humidity");
		#endif
	}
	
}

