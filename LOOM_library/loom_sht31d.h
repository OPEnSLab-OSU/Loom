// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "Adafruit_SHT31.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// #define i2c_addr_sht31d 0x44	//0x44, 0x45


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

#if i2c_addr_sht31d_0x44 == 1
	struct state_sht31d_t state_sht31d_0x44;
#endif
#if i2c_addr_sht31d_0x45 == 1
	struct state_sht31d_t state_sht31d_0x45;
#endif


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

	// if (state_sht31d.inst_sht31d.begin(i2c_addr_sht31d)) {
	#if i2c_addr_sht31d_0x44 == 1
		if (state_sht31d_0x44.inst_sht31d.begin(0x44)) {
			is_setup = true;
			LOOM_DEBUG_Println("Initialized sht31d (temp/humid)");
		} else {
			is_setup = false;
			LOOM_DEBUG_Println("Failed to initialize sht31d (temp/humid");
		}
	#endif
	#if i2c_addr_sht31d_0x45 == 1
		if (state_sht31d_0x45.inst_sht31d.begin(0x45)) {
			is_setup = true;
			LOOM_DEBUG_Println("Initialized sht31d (temp/humid)");
		} else {
			is_setup = false;
			LOOM_DEBUG_Println("Failed to initialize sht31d (temp/humid");
		}
	#endif
	
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
	char address_string[80];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/sht31d/data");
	
	#if i2c_addr_sht31d_0x44 == 1
		OSCMessage msg = OSCMessage(address_string);
		msg.add("temp").add(state_sht31d_0x44.temp);
		msg.add("humid").add(state_sht31d_0x44.humid);
		
		bndl->add(msg);
	#endif
	#if i2c_addr_sht31d_0x45 == 1
		OSCMessage msg = OSCMessage(address_string);
		msg.add("temp").add(state_sht31d_0x45.temp);
		msg.add("humid").add(state_sht31d_0x45.humid);
		
		bndl->add(msg);
	#endif

}

// Package function when not using multiplexer
#if is_multiplexer != 1
void package_sht31d(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[80];

	#if i2c_addr_sht31d_0x44 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", sht31d_0x44_name, "_temp");
		bndl->add(address_string).add(state_sht31d_0x44.temp);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", sht31d_0x44_name, "_humid");
		bndl->add(address_string ).add(state_sht31d_0x44.humid);
	#endif
	#if i2c_addr_sht31d_0x45 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", sht31d_0x45_name, "_temp");
		bndl->add(address_string).add(state_sht31d_0x45.temp);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", sht31d_0x45_name, "_humid");
		bndl->add(address_string ).add(state_sht31d_0x45.humid);
	#endif
}
#endif


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the SHT31D and stores into its state struct
// 
void measure_sht31d() 
{
	#if i2c_addr_sht31d_0x44 == 1
		float t = state_sht31d_0x44.inst_sht31d.readTemperature();
		float h = state_sht31d_0x44.inst_sht31d.readHumidity();

		if ((!isnan(t)) && (!isnan(h))) {
			state_sht31d_0x44.temp = t;
			state_sht31d_0x44.humid = h;
		} else {
			LOOM_DEBUG_Println("Failed to read temperature or humidity");
		}
	#endif

	#if i2c_addr_sht31d_0x45 == 1
		float t = state_sht31d_0x45.inst_sht31d.readTemperature();
		float h = state_sht31d_0x45.inst_sht31d.readHumidity();

		if ((!isnan(t)) && (!isnan(h))) {
			state_sht31d_0x45.temp = t;
			state_sht31d_0x45.humid = h;
		} else {
			LOOM_DEBUG_Println("Failed to read temperature or humidity");
		}
	#endif

}






