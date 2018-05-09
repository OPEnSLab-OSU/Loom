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
void package_sht31d(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void measure_sht31d();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_sht31d() {
  bool is_setup;
	if(state_sht31d.inst_sht31d.begin(i2c_addr_sht31d)) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Initialized sht31d (temp/humid)");
		#endif
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to initialize sht31d (temp/humid");
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE SHT31D ---
// Adds last read Sht31d sensor readings to provided OSC bundle
// Arguments: bndl (pointer to the bundle to be added to)
//            packet_header_string (header string to send messages with)
//            port (which port of the multiplexer the device is plugged into)
// Return:    none
void package_sht31d(OSCBundle *bndl, char packet_header_string[], uint8_t port){
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/sht31d/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("temp").add(state_sht31d.temp);
	msg.add("humid").add(state_sht31d.humid);
	
	bndl->add(msg);
}



// --- MEASURE SHT31D ---
// Gets the current sensor readings of the Sht31d and stores into its state struct
// Arguments: none
// Return:    none
void measure_sht31d() {
	float t = state_sht31d.inst_sht31d.readTemperature();
  float h = state_sht31d.inst_sht31d.readHumidity();

	if((!isnan(t)) && (!isnan(h))) {
		state_sht31d.temp = t;
		state_sht31d.humid = h;
		#if LOOM_DEBUG == 1
			Serial.print("Temp: ");
			Serial.println(state_sht31d.temp);
			Serial.print("Humidity: ");
			Serial.println(state_sht31d.humid);
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to read temperature or humidity");
		#endif
	}
}

