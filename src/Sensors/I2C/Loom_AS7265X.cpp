///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7265X.cpp
/// @brief		File for Loom_AS7265X implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_AS7265X.h"

///////////////////////////////////////////////////////////////////////////////
Loom_AS7265X::Loom_AS7265X(
		const byte			i2c_address, 
		const uint8_t		mux_port,
		const bool			use_bulb, 
		const uint8_t		gain, 
		const uint8_t		mode, 
		const uint8_t		integration_time
	)
	: LoomI2CSensor( "AS7265X", Type::AS7265X, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
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
    
    for(int i = 0; i < 18; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7265X::Loom_AS7265X(JsonArrayConst p)
	: Loom_AS7265X( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");

	// UV
	for (auto i = 0; i < 6; i++) { LPrintln("\tA: ", Values[i].retrieve<uint16_t>().value_or(0)); }
	// Color
	for (auto i = 0; i < 6; i++) { LPrintln("\tG: ", Values[i].retrieve<uint16_t>().value_or(0)); }
	// NIR
	for (auto i = 0; i < 6; i++) { LPrintln("\tR: ", Values[i].retrieve<uint16_t>().value_or(0)); }
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
	Values[0] = inst_AS7265X.getCalibratedA();
	Values[1] = inst_AS7265X.getCalibratedB();
	Values[2] = inst_AS7265X.getCalibratedC();
	Values[3] = inst_AS7265X.getCalibratedD();
	Values[4] = inst_AS7265X.getCalibratedE();
	Values[5] = inst_AS7265X.getCalibratedF();

	// Color
	Values[0+6] = inst_AS7265X.getCalibratedG();
	Values[1+6] = inst_AS7265X.getCalibratedH();
	Values[2+6] = inst_AS7265X.getCalibratedI();
	Values[3+6] = inst_AS7265X.getCalibratedJ();
	Values[4+6] = inst_AS7265X.getCalibratedK();
	Values[5+6] = inst_AS7265X.getCalibratedL();

	// NIR
	Values[0+12] = inst_AS7265X.getCalibratedR();
	Values[1+12] = inst_AS7265X.getCalibratedS();
	Values[2+12] = inst_AS7265X.getCalibratedT();
	Values[3+12] = inst_AS7265X.getCalibratedU();
	Values[4+12] = inst_AS7265X.getCalibratedV();
	Values[5+12] = inst_AS7265X.getCalibratedW();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7265X::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["a"] = Values[0].retrieve<uint16_t>().value_or(0);
	data["b"] = Values[1].retrieve<uint16_t>().value_or(0);
	data["c"] = Values[2].retrieve<uint16_t>().value_or(0);
	data["d"] = Values[3].retrieve<uint16_t>().value_or(0);
	data["e"] = Values[4].retrieve<uint16_t>().value_or(0);
	data["f"] = Values[5].retrieve<uint16_t>().value_or(0);

	data["g"] = Values[0+6].retrieve<uint16_t>().value_or(0);
	data["h"] = Values[1+6].retrieve<uint16_t>().value_or(0);
	data["i"] = Values[2+6].retrieve<uint16_t>().value_or(0);
	data["j"] = Values[3+6].retrieve<uint16_t>().value_or(0);
	data["k"] = Values[4+6].retrieve<uint16_t>().value_or(0);
	data["l"] = Values[5+6].retrieve<uint16_t>().value_or(0);

	data["r"] = Values[0+12].retrieve<uint16_t>().value_or(0);
	data["s"] = Values[1+12].retrieve<uint16_t>().value_or(0);
	data["t"] = Values[2+12].retrieve<uint16_t>().value_or(0);
	data["u"] = Values[3+12].retrieve<uint16_t>().value_or(0);
	data["v"] = Values[4+12].retrieve<uint16_t>().value_or(0);
	data["w"] = Values[5+12].retrieve<uint16_t>().value_or(0);


}

///////////////////////////////////////////////////////////////////////////////



