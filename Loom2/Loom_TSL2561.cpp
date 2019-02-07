
#include "Loom_TSL2561.h"




// --- CONSTRUCTOR ---
Loom_TSL2561::Loom_TSL2561(byte i2c_address, char* module_name, char* sensor_description, int gain, int resolution)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->gain       = gain;
	this->resolution = resolution;


	switch (i2c_address) {
		case 0x29 : inst_TSL2561 = new Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW  , 29); break;
		case 0x39 : inst_TSL2561 = new Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 39); break;
		case 0x49 : inst_TSL2561 = new Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH , 49); break;
	}

	bool setup = inst_TSL2561->begin();

	if (setup) {
		switch (gain) {
			case 1  : inst_TSL2561->setGain(TSL2561_GAIN_1X); break;
			case 16 : inst_TSL2561->setGain(TSL2561_GAIN_16X); break;
			default : inst_TSL2561->setGain(TSL2561_GAIN_1X); break;
		}

		switch (resolution) {
			case 1 : inst_TSL2561->setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS); break;
			case 2 : inst_TSL2561->setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS); break;
			case 3 : inst_TSL2561->setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); break;
		}
	}

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_TSL2561::~Loom_TSL2561() 
{

}



void Loom_TSL2561::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println4("\t", "LightIR   : ", lightIR,   " lux");
	LOOM_DEBUG_Println4("\t", "LightFull : ", lightFull, " lux");
}


void Loom_TSL2561::measure()
{
	uint16_t IR_ar[5], Full_ar[5];

	for (int i = 0; i < 5; i++) {
		inst_TSL2561->getLuminosity(&Full_ar[i], &IR_ar[i]);
	}

	lightIR   = (IR_ar[0]   + IR_ar[1]   + IR_ar[2]   + IR_ar[3]   + IR_ar[4])   / 5;
	lightFull = (Full_ar[0] + Full_ar[1] + Full_ar[2] + Full_ar[3] + Full_ar[4]) / 5;
}


void Loom_TSL2561::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	append_to_bundle_key_value(bndl, id_prefix, "LightIR"  , lightIR);
	append_to_bundle_key_value(bndl, id_prefix, "LightFull", lightFull);
}


void Loom_TSL2561::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_msg_key_value(bndl, "LightIR"  , lightIR);
	append_to_bundle_msg_key_value(bndl, "LightFull", lightFull);
}
