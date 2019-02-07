
#include "Loom_AS7265X.h"



// --- CONSTRUCTOR ---
Loom_AS7265X::Loom_AS7265X(byte i2c_address, char* module_name, char* sensor_description, bool use_bulb, byte gain, byte mode, byte integration_time)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->use_bulb 			= use_bulb;
	this->gain 				= gain;
	this->mode 				= mode;
	this->integration_time 	= integration_time;

	bool setup = inst_AS7265X.begin();

	if (setup) {

		// //There are four gain settings. It is possible to saturate the reading so don't simply jump to 64x.
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// inst_AS7265X.setGain(AS7265X_GAIN_1X); 		//Default
		// // inst_AS7265X.setGain(AS7265X_GAIN_37X); 	//This is 3.7x
		// // inst_AS7265X.setGain(AS7265X_GAIN_16X);
		// //inst_AS7265X.setGain(AS7265X_GAIN_64X);
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		// //There are four measurement modes - the datasheet describes it best
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// //inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_4CHAN); 				//Channels STUV on x51
		// //inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_4CHAN_2); 			//Channels RTUW on x51
		// //inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS); 	//All 6 channels on all devices
		// inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); 		//Default: All 6 channels, all devices, just once
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		// //Integration cycles is from 0 (2.78ms) to 255 (711ms)
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// //inst_AS7265X.setIntegrationCycles(49);	//Default 50*2.8ms = 140ms per reading
		// inst_AS7265X.setIntegrationCycles(1); 	//2*2.8ms = 5.6ms per reading
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		// //Drive current can be set for each LED
		// //4 levels: 12.5, 25, 50, and 100mA
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// //White LED has max forward current of 120mA
		// inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE); 		//Default
		// //inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_WHITE); 		//Allowed
		// //inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE); 		//Allowed 
		// //inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_WHITE); 	//Allowed

		// //UV LED has max forward current of 30mA so do not set the drive current higher
		// inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV); 		//Default

		// //IR LED has max forward current of 65mA 
		// inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR); 		//Default
		// //inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_IR); 		//Allowed
		// //inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_IR); 		//Allowed
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		// //The status indicator (Blue LED) can be enabled/disabled and have its current set
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// inst_AS7265X.enableIndicator(); //Default
		// //inst_AS7265X.disableIndicator();

		// //inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_1MA);
		// //inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_2MA);
		// //inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_4MA);
		// inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_8MA); 	//Default
		// //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	}

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_AS7265X::~Loom_AS7265X() 
{

}



void Loom_AS7265X::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");

	// UV
	for (int i = 0; i < 6; i++) { LOOM_DEBUG_Println3("\t", "A: ", uv[i]); }
	// Color
	for (int i = 0; i < 6; i++) { LOOM_DEBUG_Println3("\t", "G: ", color[i]); }
	// NIR
	for (int i = 0; i < 6; i++) { LOOM_DEBUG_Println3("\t", "R: ", nir[i]); }
}


void Loom_AS7265X::measure()
{
	if (use_bulb) {
		inst_AS7265X.takeMeasurementsWithBulb();
	} else {
		inst_AS7265X.takeMeasurements();
	}
	
	// UV
	uv[0] = inst_AS7265X.getCalibratedA();
	uv[1] = inst_AS7265X.getCalibratedB();
	uv[2] = inst_AS7265X.getCalibratedC();
	uv[3] = inst_AS7265X.getCalibratedD();
	uv[4] = inst_AS7265X.getCalibratedE();
	uv[5] = inst_AS7265X.getCalibratedF();

	// Color
	color[0] = inst_AS7265X.getCalibratedG();
	color[1] = inst_AS7265X.getCalibratedH();
	color[2] = inst_AS7265X.getCalibratedI();
	color[3] = inst_AS7265X.getCalibratedJ();
	color[4] = inst_AS7265X.getCalibratedK();
	color[5] = inst_AS7265X.getCalibratedL();

	// NIR
	nir[0] = inst_AS7265X.getCalibratedR();
	nir[1] = inst_AS7265X.getCalibratedS();
	nir[2] = inst_AS7265X.getCalibratedT();
	nir[3] = inst_AS7265X.getCalibratedU();
	nir[4] = inst_AS7265X.getCalibratedV();
	nir[5] = inst_AS7265X.getCalibratedW();
}


void Loom_AS7265X::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	// UV
	append_to_bundle_key_value(bndl, id_prefix, "a", uv[0]);
	append_to_bundle_key_value(bndl, id_prefix, "b", uv[1]);
	append_to_bundle_key_value(bndl, id_prefix, "c", uv[2]);
	append_to_bundle_key_value(bndl, id_prefix, "d", uv[3]);
	append_to_bundle_key_value(bndl, id_prefix, "e", uv[4]);
	append_to_bundle_key_value(bndl, id_prefix, "f", uv[5]);
	// Color
	append_to_bundle_key_value(bndl, id_prefix, "g", color[0]); 
	append_to_bundle_key_value(bndl, id_prefix, "h", color[1]); 
	append_to_bundle_key_value(bndl, id_prefix, "i", color[2]); 
	append_to_bundle_key_value(bndl, id_prefix, "j", color[3]); 
	append_to_bundle_key_value(bndl, id_prefix, "k", color[4]); 
	append_to_bundle_key_value(bndl, id_prefix, "l", color[5]); 

	// NIR
	append_to_bundle_key_value(bndl, id_prefix, "r", nir[0]);
	append_to_bundle_key_value(bndl, id_prefix, "s", nir[1]);
	append_to_bundle_key_value(bndl, id_prefix, "t", nir[2]);
	append_to_bundle_key_value(bndl, id_prefix, "u", nir[3]);
	append_to_bundle_key_value(bndl, id_prefix, "v", nir[4]);
	append_to_bundle_key_value(bndl, id_prefix, "w", nir[5]);

}


void Loom_AS7265X::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	// UV
	append_to_bundle_msg_key_value(bndl, "a", uv[0]);
	append_to_bundle_msg_key_value(bndl, "b", uv[1]);
	append_to_bundle_msg_key_value(bndl, "c", uv[2]);
	append_to_bundle_msg_key_value(bndl, "d", uv[3]);
	append_to_bundle_msg_key_value(bndl, "e", uv[4]);
	append_to_bundle_msg_key_value(bndl, "f", uv[5]);
	// Color
	append_to_bundle_msg_key_value(bndl, "g", color[0]); 
	append_to_bundle_msg_key_value(bndl, "h", color[1]); 
	append_to_bundle_msg_key_value(bndl, "i", color[2]); 
	append_to_bundle_msg_key_value(bndl, "j", color[3]); 
	append_to_bundle_msg_key_value(bndl, "k", color[4]); 
	append_to_bundle_msg_key_value(bndl, "l", color[5]); 

	// NIR
	append_to_bundle_msg_key_value(bndl, "r", nir[0]);
	append_to_bundle_msg_key_value(bndl, "s", nir[1]);
	append_to_bundle_msg_key_value(bndl, "t", nir[2]);
	append_to_bundle_msg_key_value(bndl, "u", nir[3]);
	append_to_bundle_msg_key_value(bndl, "v", nir[4]);
	append_to_bundle_msg_key_value(bndl, "w", nir[5]);
}


void Loom_AS7265X::enable_bulb(bool e)
{
	use_bulb = e;
}

// 1 to 64x
void Loom_AS7265X::set_gain(byte gain) 
{
	inst_AS7265X.setGain(gain);
}

//4 channel, other 4 channel, 6 chan, or 6 chan one shot
void Loom_AS7265X::set_mode(byte mode) 
{
	inst_AS7265X.setMeasurementMode(mode);
}

//50 * 2.8ms = 140ms. 0 to 255 is valid.  (49 is default)
//If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.
void Loom_AS7265X::set_integration_time(byte time) 
{
	inst_AS7265X.setIntegrationCycles(time);
}
