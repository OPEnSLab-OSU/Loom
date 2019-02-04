
#include "Loom_AS7265X.h"



// --- CONSTRUCTOR ---
Loom_AS7265X::Loom_AS7265X(byte i2c_address, char* module_name, char* sensor_description, bool use_bulb)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->use_bulb = use_bulb;

	bool setup = inst_7265X.begin(i2c_address);


	//There are four gain settings. It is possible to saturate the reading so don't simply jump to 64x.
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	inst_AS7265X.setGain(AS7265X_GAIN_1X); 		//Default
	// inst_AS7265X.setGain(AS7265X_GAIN_37X); 	//This is 3.7x
	// inst_AS7265X.setGain(AS7265X_GAIN_16X);
	//inst_AS7265X.setGain(AS7265X_GAIN_64X);
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//There are four measurement modes - the datasheet describes it best
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_4CHAN); 				//Channels STUV on x51
	//inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_4CHAN_2); 			//Channels RTUW on x51
	//inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS); 	//All 6 channels on all devices
	inst_AS7265X.setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); 		//Default: All 6 channels, all devices, just once
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//Integration cycles is from 0 (2.78ms) to 255 (711ms)
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//inst_AS7265X.setIntegrationCycles(49);	//Default 50*2.8ms = 140ms per reading
	inst_AS7265X.setIntegrationCycles(1); 	//2*2.8ms = 5.6ms per reading
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//Drive current can be set for each LED
	//4 levels: 12.5, 25, 50, and 100mA
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	//White LED has max forward current of 120mA
	inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE); 		//Default
	//inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_WHITE); 		//Allowed
	//inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_WHITE); 		//Allowed 
	//inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_100MA, AS7265x_LED_WHITE); 	//Allowed

	//UV LED has max forward current of 30mA so do not set the drive current higher
	inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV); 		//Default

	//IR LED has max forward current of 65mA 
	inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR); 		//Default
	//inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_25MA, AS7265x_LED_IR); 		//Allowed
	//inst_AS7265X.setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_50MA, AS7265x_LED_IR); 		//Allowed
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	//The status indicator (Blue LED) can be enabled/disabled and have its current set
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	inst_AS7265X.enableIndicator(); //Default
	//inst_AS7265X.disableIndicator();

	//inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_1MA);
	//inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_2MA);
	//inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_4MA);
	inst_AS7265X.setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_8MA); 	//Default
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


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
	if (as7265X_use_bulb) {
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
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "a"+i, uv[i]); }
	// Color
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "g"+i, color[i]); }
	// NIR
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "r"+i, nir[i]); }
}


void Loom_AS7265X::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	// UV
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "a"+i, uv[i]); }
	// Color
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "g"+i, color[i]); }
	// NIR
	for (int i = 0; i < 6; i++) { append_to_bundle_key_value(bndl, id_prefix, "r"+i, nir[i]); }
}


void Loom_AS7265X::use_bulb(bool e)
{
	use_bulb = e;
}
