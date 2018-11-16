// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include "SparkFun_AS7265X.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

bool as7265X_use_bulb = false;

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_as7265X_t {
	int a, b, c, d, e, f;
	int g, h, i, j, k, l;
	int r, s, t, u, v, w;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

#if i2c_addr_as7265X_0x49 == 1
	struct state_as7265X_t state_as7265X_0x49;
	AS7265X inst_as7265X_0x49;
#endif


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_as7265X();
void package_as7265X(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_as7265X(OSCBundle *bndl, char packet_header_string[]);
void measure_as7265X();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any AS7265X setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_as7265X() 
{
	LOOM_DEBUG_Println("Setting up AS7265X");

	#if i2c_addr_as7265X_0x49 == 1
		if ( !inst_as7265X_0x49.begin() ) {
			LOOM_DEBUG_Println("No AS7265X sensor found");
			while (1);
		}
	#endif
	
	LOOM_DEBUG_Println("AS7265X setup complete");
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE AS7265X --- (Multiplexer Version)
// 
// Adds last read AS7265X sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_as7265X(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/as7265X/data");
	
	OSCMessage msg = OSCMessage(address_string);
	
	#if i2c_addr_as7265X_0x49 == 1
		msg.add("a").add( (int32_t)state_as7265X_0x49.a );
		msg.add("b").add( (int32_t)state_as7265X_0x49.b );
		msg.add("c").add( (int32_t)state_as7265X_0x49.c );
		msg.add("d").add( (int32_t)state_as7265X_0x49.d );
		msg.add("e").add( (int32_t)state_as7265X_0x49.e );
		msg.add("f").add( (int32_t)state_as7265X_0x49.f );

		msg.add("g").add( (int32_t)state_as7265X_0x49.g );
		msg.add("h").add( (int32_t)state_as7265X_0x49.h );
		msg.add("i").add( (int32_t)state_as7265X_0x49.i );
		msg.add("j").add( (int32_t)state_as7265X_0x49.j );
		msg.add("k").add( (int32_t)state_as7265X_0x49.k );
		msg.add("l").add( (int32_t)state_as7265X_0x49.l );

		msg.add("r").add( (int32_t)state_as7265X_0x49.r );
		msg.add("s").add( (int32_t)state_as7265X_0x49.s );
		msg.add("t").add( (int32_t)state_as7265X_0x49.t );
		msg.add("u").add( (int32_t)state_as7265X_0x49.u );
		msg.add("v").add( (int32_t)state_as7265X_0x49.v );
		msg.add("w").add( (int32_t)state_as7265X_0x49.w );
	#endif

	bndl->add(msg);
}



// Package function when not using multiplexer
#if is_multiplexer != 1
void package_as7265X(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if i2c_addr_as7265X_0x49 == 1

		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "a");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.a );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "b");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.b );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "c");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.c );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "d");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.d );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "e");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.e );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "f");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.f );

		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "g");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.g );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "h");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.h );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "i");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.i );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "j");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.j );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "k");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.k );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "l");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.l );

		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "r");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.r );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "s");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.s );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "t");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.t );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "u");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.u );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "v");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.v );
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", as7265X_0x49_name, "w");
			bndl->add(address_string).add( (int32_t)state_as7265X_0x49.w );
		
	#endif
}
#endif



// --- MEASURE AS7265X ---
//
// Gets the current sensor readings of the AS7265Xs and stores into its state struct
// Takes samples 5 times for average
// 
void measure_as7265X() 
{

	#if i2c_addr_as7265X_0x49 == 1

		if (as7265X_use_bulb) {
			inst_as7265X_0x49.takeMeasurementsWithBulb();
		} else {
			inst_as7265X_0x49.takeMeasurements();
		}

		state_as7265X_0x49.a = inst_as7265X_0x49.getCalibratedA();
		state_as7265X_0x49.b = inst_as7265X_0x49.getCalibratedB();
		state_as7265X_0x49.c = inst_as7265X_0x49.getCalibratedC();
		state_as7265X_0x49.d = inst_as7265X_0x49.getCalibratedD();
		state_as7265X_0x49.e = inst_as7265X_0x49.getCalibratedE();
		state_as7265X_0x49.f = inst_as7265X_0x49.getCalibratedF();

		state_as7265X_0x49.g = inst_as7265X_0x49.getCalibratedG();
		state_as7265X_0x49.h = inst_as7265X_0x49.getCalibratedH();
		state_as7265X_0x49.i = inst_as7265X_0x49.getCalibratedI();
		state_as7265X_0x49.j = inst_as7265X_0x49.getCalibratedJ();
		state_as7265X_0x49.k = inst_as7265X_0x49.getCalibratedK();
		state_as7265X_0x49.l = inst_as7265X_0x49.getCalibratedL();

		state_as7265X_0x49.r = inst_as7265X_0x49.getCalibratedR();
		state_as7265X_0x49.s = inst_as7265X_0x49.getCalibratedS();
		state_as7265X_0x49.t = inst_as7265X_0x49.getCalibratedT();
		state_as7265X_0x49.u = inst_as7265X_0x49.getCalibratedU();
		state_as7265X_0x49.v = inst_as7265X_0x49.getCalibratedV();
		state_as7265X_0x49.w = inst_as7265X_0x49.getCalibratedW();

		// inst_as7265X_0x49.printMeasurements();

	#endif
}

	






