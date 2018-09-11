// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
// #include <Wire.h>
#include "HX711.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

#define DOUT 12 //connecting the out and clock pins for the load cell
#define CLK  13

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_hx711_t {
	float loadcell;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_hx711_t state_hx711;

HX711 inst_hx711(DOUT, CLK);

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_hx711();
void package_hx711(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_hx711(OSCBundle *bndl, char packet_header_string[]);
void measure_hx711();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any SHT31D setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_hx711() {

	LOOM_DEBUG_Println("Setting up HX711");

	bool is_setup;

	//This value is obtained by using the Calibration sketch
	inst_hx711.set_scale(hx711_calibration);

	 //Assuming there is no weight on the scale at start up, reset the scale to 0
	inst_hx711.tare();

	// Go into low power mode
	inst_hx711.power_down(); 

	LOOM_DEBUG_Println("HX711 setup complete");

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
void package_hx711(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/hx711/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("loadcell").add(state_hx711.loadcell);
	
	bndl->add(msg);
}

// Package function when not using multiplexer
void package_hx711(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s", packet_header_string, "/hx711_loadcell");
	bndl->add(address_string).add(state_hx711.loadcell);
}


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the SHT31D and stores into its state struct
// 
void measure_hx711() 
{
    inst_hx711.power_up();
	state_hx711.loadcell = inst_hx711.get_units(5);
	inst_hx711.power_down();
}






