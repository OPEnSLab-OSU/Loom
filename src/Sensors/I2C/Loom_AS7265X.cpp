
#include "Loom_AS7265X.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7265X::Loom_AS7265X(
		byte			i2c_address, 
		uint8_t			mux_port,
		const char*		module_name, 
		bool			use_bulb, 
		uint8_t			gain, 
		uint8_t			mode, 
		uint8_t			integration_time
	)
	: LoomI2CSensor( module_name, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
	this->module_type = LoomModule::Type::AS7265X;

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

	if (!setup) active = false;

	print_module_label();
	LPrintln("\tInitialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7265X::Loom_AS7265X(JsonArrayConst p)
	: Loom_AS7265X( EXPAND_ARRAY(p, 7) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");

	// UV
	for (auto i = 0; i < 6; i++) { LPrintln("\tA: ", uv[i]); }
	// Color
	for (auto i = 0; i < 6; i++) { LPrintln("\tG: ", color[i]); }
	// NIR
	for (auto i = 0; i < 6; i++) { LPrintln("\tR: ", nir[i]); }
}

///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::package(JsonObject json)
{
	package_json(json, module_name, 
		// UV
		"a", uv[0],
		"b", uv[1],
		"c", uv[2],
		"d", uv[3],
		"e", uv[4],
		"f", uv[5],

		// Color
		"g", color[0],
		"h", color[1],
		"i", color[2],
		"j", color[3],
		"k", color[4],
		"l", color[5],

		// NIR
		"r", nir[0],
		"s", nir[1],
		"t", nir[2],
		"u", nir[3],
		"v", nir[4],
		"w", nir[5]
	);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::enable_bulb(bool e)
{
	use_bulb = e;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::set_gain(uint8_t gain) 
{
	inst_AS7265X.setGain(gain);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::set_mode(uint8_t mode) 
{
	inst_AS7265X.setMeasurementMode(mode);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::set_integration_time(uint8_t time) 
{
	inst_AS7265X.setIntegrationCycles(time);
}

///////////////////////////////////////////////////////////////////////////////
