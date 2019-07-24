
#include "Loom_TSL2561.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_TSL2561::Loom_TSL2561(
		byte			i2c_address, 
		int				mux_port,
		const char*		module_name, 
		int				gain, 
		int				resolution
	)
	: LoomI2CSensor( module_name, i2c_address, mux_port )
{
	this->module_type = LoomModule::Type::TSL2561;

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

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TSL2561::Loom_TSL2561(JsonArrayConst p)
	: Loom_TSL2561( EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_TSL2561::~Loom_TSL2561() 
{
	delete inst_TSL2561;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2561::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "LightIR   : ", lightIR,   " lux");
	LPrintln("\t", "LightFull : ", lightFull, " lux");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2561::measure()
{
	uint16_t IR_ar[5], Full_ar[5];

	for (int i = 0; i < 5; i++) {
		inst_TSL2561->getLuminosity(&Full_ar[i], &IR_ar[i]);
	}

	lightIR   = (IR_ar[0]   + IR_ar[1]   + IR_ar[2]   + IR_ar[3]   + IR_ar[4])   / 5;
	lightFull = (Full_ar[0] + Full_ar[1] + Full_ar[2] + Full_ar[3] + Full_ar[4]) / 5;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2561::package(JsonObject json)
{
	int lux = inst_TSL2561->calculateLux(lightFull, lightIR);
	
	package_json(json, module_name, 
		"IR",	lightIR,
		"Full",	lightFull,
		"Lux",	lux
	);
}

///////////////////////////////////////////////////////////////////////////////


