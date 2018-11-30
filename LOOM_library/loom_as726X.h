// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_as726X_0x49.violet
// state_as726X_0x49.blue
// state_as726X_0x49.green
// state_as726X_0x49.yellow
// state_as726X_0x49.orange
// state_as726X_0x49.red

// state_as726X_0x49.nir_r
// state_as726X_0x49.nir_s
// state_as726X_0x49.nir_t
// state_as726X_0x49.nir_u
// state_as726X_0x49.nir_v
// state_as726X_0x49.nir_w



// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include "AS726X.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
byte GAIN = 3;
byte MEASUREMENT_MODE = 3;

bool as726X_use_bulb = false;

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_as726X_t {
	int violet, blue, green, yellow, orange, red;
	int nir_r, nir_s, nir_t, nir_u, nir_v, nir_w;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

#if i2c_addr_as726X_0x49 == 1
	struct state_as726X_t state_as726X_0x49;
	AS726X inst_as726X_0x49;
#endif


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_as726X();
void package_as726X(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_as726X(OSCBundle *bndl, char packet_header_string[]);
void measure_as726X();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any AS726X setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_as726X() 
{
	LOOM_DEBUG_Println("Setting up AS726X");

	inst_as726X_0x49.begin(Wire, GAIN, MEASUREMENT_MODE);

	// #if i2c_addr_as726X_0x49 == 1
	// 	if ( !inst_as726X_0x49.begin(Wire, GAIN, MEASUREMENT_MODE) ) {
	// 		LOOM_DEBUG_Println("No as726X sensor found");
	// 		while (1);
	// 	}
	// #endif
	
	LOOM_DEBUG_Println("AS726X setup complete");
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE AS726X --- (Multiplexer Version)
// 
// Adds last read AS726X sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_as726X(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/as726X/data");
	
	OSCMessage msg = OSCMessage(address_string);
	
	#if i2c_addr_as726X_0x49 == 1
		if ( inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7262) {
			msg.add("violet").add( (int32_t)state_as726X_0x49.violet );
			msg.add("blue"  ).add( (int32_t)state_as726X_0x49.blue );
			msg.add("green" ).add( (int32_t)state_as726X_0x49.green );
			msg.add("yellow").add( (int32_t)state_as726X_0x49.yellow );
			msg.add("orange").add( (int32_t)state_as726X_0x49.orange );
			msg.add("red"   ).add( (int32_t)state_as726X_0x49.red );
		} else if (inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7263) {
			msg.add("nir_r").add( (int32_t)state_as726X_0x49.nir_r );
			msg.add("nir_s").add( (int32_t)state_as726X_0x49.nir_s );
			msg.add("nir_t").add( (int32_t)state_as726X_0x49.nir_t );
			msg.add("nir_u").add( (int32_t)state_as726X_0x49.nir_u );
			msg.add("nir_v").add( (int32_t)state_as726X_0x49.nir_v );
			msg.add("nir_w").add( (int32_t)state_as726X_0x49.nir_w );
		}
	#endif

	bndl->add(msg);
}



// Package function when not using multiplexer
#if is_multiplexer != 1
void package_as726X(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if i2c_addr_as726X_0x49 == 1

		if ( inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7262) {
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "violet");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.violet );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "blue");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.blue );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "green");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.green );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "yellow");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.yellow );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "orange");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.orange );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "red");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.red );
		} else if (inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7263) {
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_r");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_r );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_s");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_s );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_t");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_t );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_u");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_u );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_v");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_v );
			sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as726X_0x49_name, "nir_w");
			bndl->add(address_string).add( (int32_t)state_as726X_0x49.nir_w );	
		}


		
	#endif
}
#endif



// --- MEASURE AS726X ---
//
// Gets the current sensor readings of the AS726Xs and stores into its state struct
// Takes samples 5 times for average
// 
void measure_as726X() 
{

	#if i2c_addr_as726X_0x49 == 1


		if (as726X_use_bulb) {
			inst_as726X_0x49.takeMeasurementsWithBulb();
		} else {
			inst_as726X_0x49.takeMeasurements();
		}


		if ( inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7262) {
			state_as726X_0x49.violet = inst_as726X_0x49.getViolet();
			state_as726X_0x49.blue   = inst_as726X_0x49.getBlue();
			state_as726X_0x49.green  = inst_as726X_0x49.getGreen();
			state_as726X_0x49.yellow = inst_as726X_0x49.getYellow();
			state_as726X_0x49.orange = inst_as726X_0x49.getOrange();
			state_as726X_0x49.red    = inst_as726X_0x49.getRed();	
		} else if ( inst_as726X_0x49.virtualReadRegister(AS726x_HW_VERSION) == SENSORTYPE_AS7263) {
			state_as726X_0x49.nir_r  = inst_as726X_0x49.getR();
			state_as726X_0x49.nir_s  = inst_as726X_0x49.getS();
			state_as726X_0x49.nir_t  = inst_as726X_0x49.getT();
			state_as726X_0x49.nir_u  = inst_as726X_0x49.getU();
			state_as726X_0x49.nir_v  = inst_as726X_0x49.getV();
			state_as726X_0x49.nir_w  = inst_as726X_0x49.getW();	
		}

		// inst_as726X_0x49.printMeasurements();

	#endif
}

	






