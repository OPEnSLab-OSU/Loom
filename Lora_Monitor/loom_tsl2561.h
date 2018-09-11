// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

// Fix this, as this might not work correctly with multiplexer

#if is_tsl2561_low   == 1
	#define i2c_addr_tsl2561 0x29 // Wired to ground
#endif
#if is_tsl2561_float == 1
	#define i2c_addr_tsl2561 0x49 // Wired to vcc
#endif
#if is_tsl2561_high  == 1
	#define i2c_addr_tsl2561 0x39 // floating
#endif



// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_tsl2561_t {
	#if is_tsl2561_low   == 1
		int lightIR_l;
		int lightFull_l;
	#endif
	#if is_tsl2561_float == 1
		int lightIR_f;
		int lightFull_f;
	#endif
	#if is_tsl2561_high  == 1
		int lightIR_h;
		int lightFull_h;
	#endif
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_tsl2561_t state_tsl2561;

#if is_tsl2561_low   == 1
	Adafruit_TSL2561_Unified inst_tsl2561_low   = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW,   25611);
#endif
#if is_tsl2561_float == 1
	Adafruit_TSL2561_Unified inst_tsl2561_float = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 25612);
#endif
#if is_tsl2561_high  == 1
	Adafruit_TSL2561_Unified inst_tsl2561_high  = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH,  25613);
#endif


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tsl2561();
void package_tsl2561(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_tsl2561(OSCBundle *bndl, char packet_header_string[]);
void measure_tsl2561();
void details_tsl2561();
void configure_tsl2561();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any TSL2561 setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_tsl2561() 
{
	LOOM_DEBUG_Println("Setting up TSL2561");

	#if is_tsl2561_low   == 1
		if (!inst_tsl2561_low.begin()) {
			LOOM_DEBUG_Println("No TSL2561 sensor found with I2C selection to ground ... check your wiring?");
			while (1);
		}
	#endif
	#if is_tsl2561_float == 1
		if (!inst_tsl2561_float.begin()) {
			LOOM_DEBUG_Println("No TSL2561 sensor found with I2C selection to float ... check your wiring?");
			while (1);
		}
	#endif
	#if is_tsl2561_high  == 1
		if (!inst_tsl2561_high.begin()) {
			LOOM_DEBUG_Println("No TSL2561 sensor found with I2C selection to high ... check your wiring?");
			while (1);
		}
	#endif

	configure_tsl2561();

	LOOM_DEBUG_Println("TSL2561 setup complete");
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
void package_tsl2561(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/tsl2561/data");
	
	OSCMessage msg = OSCMessage(address_string);
	

	#if is_tsl2561_low   == 1
		msg.add("lightIR_low"    ).add( (int32_t)state_tsl2561.lightIR_l );
		msg.add("lightFull_low"  ).add( (int32_t)state_tsl2561.lightFull_l );
	#endif
	#if is_tsl2561_float == 1
		msg.add("lightIR_float"  ).add( (int32_t)state_tsl2561.lightIR_f );
		msg.add("lightFull_float").add( (int32_t)state_tsl2561.lightFull_f );
	#endif
	#if is_tsl2561_high  == 1
		msg.add("lightIR_high"   ).add( (int32_t)state_tsl2561.lightIR_h );
		msg.add("lightFull_high" ).add( (int32_t)state_tsl2561.lightFull_h );
	#endif


	bndl->add(msg);
}

// Package function when not using multiplexer
void package_tsl2561(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if is_tsl2561_low   == 1
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_ir_low");
		bndl->add(address_string).add( (int32_t)state_tsl2561.lightIR_l );
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_full_low");
		bndl->add(address_string ).add( (int32_t)state_tsl2561.lightFull_l );
	#endif
	#if is_tsl2561_float == 1
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_ir_float");
		bndl->add(address_string).add( (int32_t)state_tsl2561.lightIR_f );
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_full_float");
		bndl->add(address_string ).add( (int32_t)state_tsl2561.lightFull_f );
	#endif
	#if is_tsl2561_high  == 1
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_ir_high");
		bndl->add(address_string).add( (int32_t)state_tsl2561.lightIR_h );
		sprintf(address_string, "%s%s", packet_header_string, "/tsl2561_full_high");
		bndl->add(address_string ).add( (int32_t)state_tsl2561.lightFull_h );
	#endif
}


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the TSL2561s and stores into its state struct
// Takes samples 5 times for average
// 
void measure_tsl2561() 
{
	uint16_t lightIR_ar[5], lightFull_ar[5];

	#if is_tsl2561_low   == 1
		for (int i = 0; i < 5; i++) {
			inst_tsl2561_low.getLuminosity(&lightFull_ar[i], &lightIR_ar[i]);
		}
		state_tsl2561.lightIR_l   = (lightIR_ar[0]   + lightIR_ar[1]   + lightIR_ar[2]   + lightIR_ar[3]   + lightIR_ar[4])   / 5;
		state_tsl2561.lightFull_l = (lightFull_ar[0] + lightFull_ar[1] + lightFull_ar[2] + lightFull_ar[3] + lightFull_ar[4]) / 5;
	#endif
	#if is_tsl2561_float == 1
		for (int i = 0; i < 5; i++) {
			inst_tsl2561_float.getLuminosity(&lightFull_ar[i], &lightIR_ar[i]);
		}
		state_tsl2561.lightIR_f   = (lightIR_ar[0]   + lightIR_ar[1]   + lightIR_ar[2]   + lightIR_ar[3]   + lightIR_ar[4])   / 5;
		state_tsl2561.lightFull_f = (lightFull_ar[0] + lightFull_ar[1] + lightFull_ar[2] + lightFull_ar[3] + lightFull_ar[4]) / 5;
	#endif
	#if is_tsl2561_high  == 1
		for (int i = 0; i < 5; i++) {
			inst_tsl2561_high.getLuminosity(&lightFull_ar[i], &lightIR_ar[i]);
		}
		state_tsl2561.lightIR_f   = (lightIR_ar[0]   + lightIR_ar[1]   + lightIR_ar[2]   + lightIR_ar[3]   + lightIR_ar[4])   / 5;
		state_tsl2561.lightFull_f = (lightFull_ar[0] + lightFull_ar[1] + lightFull_ar[2] + lightFull_ar[3] + lightFull_ar[4]) / 5;
	#endif
}



// --- DISPLAY SENSOR DETAILS --- 
//
// Displays information about the connected
// TS2561 sensor(s)
//
void details_tsl2561()
{
	sensor_t sensor;

	for (int i = 0; i < 3; i++) {
		switch(i) {
			case 0: 
				#if is_tsl2561_low == 1
					inst_tsl2561_low.getSensor(&sensor);
					break;
				#else
					continue;
				#endif
			case 1:
				#if is_tsl2561_float == 1
					inst_tsl2561_float.getSensor(&sensor);
					break;
				#else
					continue;
				#endif
			case 2:
				#if is_tsl2561_high == 1
					inst_tsl2561_high.getSensor(&sensor);
					break;
				#else
					continue;
				#endif
		} // of switch

		LOOM_DEBUG_Println("------------------------------------");
		LOOM_DEBUG_Println4("Sensor ", i+1, ": ", sensor.name);
		LOOM_DEBUG_Println2("Driver Ver:   ", sensor.version);
		LOOM_DEBUG_Println2("Unique ID:    ", sensor.sensor_id);
		LOOM_DEBUG_Println3("Max Value:    ", sensor.max_value, " lux");
		LOOM_DEBUG_Println3("Min Value:    ", sensor.min_value, " lux");
		LOOM_DEBUG_Println3("Resolution:   ", sensor.resolution," lux");  
		LOOM_DEBUG_Println("------------------------------------");
	}
}


void configure_tsl2561()
{
	// --- Gain settings
	// You can also manually set the gain or enable auto-gain support 
	// TSL2561_GAIN_1X  : No gain  (use in bright light to avoid sensor saturation) 
	// TSL2561_GAIN_16X : 16x gain (use in low light to boost sensitivity)
	// tsl.enableAutoRange(true);    // Auto-gain ... switches automatically between 1x and 16x 


	// --- Integration time
	// Changing the integration time gives you better sensor resolution (402ms = 16-bit data) 
	// TSL2561_INTEGRATIONTIME_101MS) : medium resolution and speed  
	// TSL2561_INTEGRATIONTIME_402MS) : 16-bit data but slowest conversions 


	#if is_tsl2561_low   == 1
		inst_tsl2561_low.setGain(TSL2561_GAIN_1X);
		#if  tsl2561_res == 1
			inst_tsl2561_low.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
		#elif tsl2561_res == 2
			inst_tsl2561_low.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
		#else 
			inst_tsl2561_low.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
		#endif
	#endif
	#if is_tsl2561_float == 1
		inst_tsl2561_float.setGain(TSL2561_GAIN_1X);
		// inst_tsl2561_float.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
		#if  tsl2561_res == 1
			inst_tsl2561_float.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
		#elif tsl2561_res == 2
			inst_tsl2561_float.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
		#else 
			inst_tsl2561_float.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
		#endif
	#endif
	#if is_tsl2561_high  == 1
		inst_tsl2561_high.setGain(TSL2561_GAIN_1X);
		// inst_tsl2561_high.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);

		#if  tsl2561_res == 1
			inst_tsl2561_high.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);;
		#elif tsl2561_res == 2
			inst_tsl2561_high.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);;
		#else 
			inst_tsl2561_high.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);;
		#endif
	#endif


}


