// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "Adafruit_SHT31.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_sht31d 0x44	//0x44, 0x45


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
void package_sht31d(OSCBundle *bndl, char packet_header_string[]);
void measure_sht31d();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any SHT31D setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_sht31d() {
	bool is_setup;

	if (state_sht31d.inst_sht31d.begin(i2c_addr_sht31d)) {
		is_setup = true;
		LOOM_DEBUG_Println("Initialized sht31d (temp/humid)");
	} else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to initialize sht31d (temp/humid");
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE SHT31D --- (Multiplexer Version)
// 
// Adds last read SHT31D sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_sht31d(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/sht31d/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("temp").add(state_sht31d.temp);
	msg.add("humid").add(state_sht31d.humid);
	
	bndl->add(msg);
}

void package_sht31d(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(addressString, "%s%s", packet_header_string, "/sht31d_temp");
	bndl->add(addressString).add(state_sht31d.temp);
	sprintf(addressString, "%s%s", packet_header_string, "/sht31d_humid");
	bndl->add(addressString ).add(state_sht31d.humid);
}


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the SHT31D and stores into its state struct
// 
void measure_sht31d() 
{
	float t = state_sht31d.inst_sht31d.readTemperature();
	float h = state_sht31d.inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		state_sht31d.temp = t;
		state_sht31d.humid = h;
		LOOM_DEBUG_Println2("Temp: ",     state_sht31d.temp);
		LOOM_DEBUG_Println2("Humidity: ", state_sht31d.humid);
	} else {
		LOOM_DEBUG_Println("Failed to read temperature or humidity");
	}
}






