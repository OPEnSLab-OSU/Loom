///////////////////////////////////////////////////////////////////////////////
///
/// @file		AS7265X.cpp
/// @brief		File for AS7265X implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Module_Factory.h"
#include "AS7265X.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Loom::AS7265X::AS7265X(
		const byte			i2c_address,
		const uint8_t		mux_port,
		const bool			use_bulb,
		const uint8_t		gain,
		const uint8_t		mode,
		const uint8_t		integration_time
	)
	: I2CSensor("AS7265X", i2c_address, mux_port)
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
  LMark;
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
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom::AS7265X::AS7265X(JsonArrayConst p)
	: AS7265X(EXPAND_ARRAY(p, 6)) {}

///////////////////////////////////////////////////////////////////////////////
void Loom::AS7265X::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;

	// UV
	for (auto i = 0; i < 6; i++) { LPrintln("\tA: ", uv[i]); }
	// Color
	for (auto i = 0; i < 6; i++) { LPrintln("\tG: ", color[i]); }
	// NIR
	for (auto i = 0; i < 6; i++) { LPrintln("\tR: ", nir[i]); }
}

///////////////////////////////////////////////////////////////////////////////
void Loom::AS7265X::measure()
{
  LMark;
	if (use_bulb) {
   	LMark;
		inst_AS7265X.takeMeasurementsWithBulb();
	} else {
		LMark;
		inst_AS7265X.takeMeasurements();
	}

	// UV
	uv[0] = inst_AS7265X.getCalibratedA();
  LMark;
	uv[1] = inst_AS7265X.getCalibratedB();
  LMark;
	uv[2] = inst_AS7265X.getCalibratedC();
  LMark;
	uv[3] = inst_AS7265X.getCalibratedD();
  LMark;
	uv[4] = inst_AS7265X.getCalibratedE();
  LMark;
	uv[5] = inst_AS7265X.getCalibratedF();
  LMark;

	// Color
	color[0] = inst_AS7265X.getCalibratedG();
  LMark;
	color[1] = inst_AS7265X.getCalibratedH();
  LMark;
	color[2] = inst_AS7265X.getCalibratedI();
  LMark;
	color[3] = inst_AS7265X.getCalibratedJ();
  LMark;
	color[4] = inst_AS7265X.getCalibratedK();
  LMark;
	color[5] = inst_AS7265X.getCalibratedL();
  LMark;

	// NIR
	nir[0] = inst_AS7265X.getCalibratedR();
  LMark;
	nir[1] = inst_AS7265X.getCalibratedS();
  LMark;
	nir[2] = inst_AS7265X.getCalibratedT();
  LMark;
	nir[3] = inst_AS7265X.getCalibratedU();
  LMark;
	nir[4] = inst_AS7265X.getCalibratedV();
  LMark;
	nir[5] = inst_AS7265X.getCalibratedW();
}

///////////////////////////////////////////////////////////////////////////////
void Loom::AS7265X::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["a"] = uv[0];
  LMark;
	data["b"] = uv[1];
  LMark;
	data["c"] = uv[2];
  LMark;
	data["d"] = uv[3];
  LMark;
	data["e"] = uv[4];
  LMark;
	data["f"] = uv[5];
  LMark;

	data["g"] = color[0];
  LMark;
	data["h"] = color[1];
  LMark;
	data["i"] = color[2];
  LMark;
	data["j"] = color[3];
  LMark;
	data["k"] = color[4];
  LMark;
	data["l"] = color[5];
  LMark;

	data["r"] = nir[0];
  LMark;
	data["s"] = nir[1];
  LMark;
	data["t"] = nir[2];
  LMark;
	data["u"] = nir[3];
  LMark;
	data["v"] = nir[4];
  LMark;
	data["w"] = nir[5];
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
