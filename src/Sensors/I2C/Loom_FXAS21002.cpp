
#include "Loom_FXAS21002.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(
		byte			i2c_address, 
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = LoomModule::Type::FXAS21002;

	inst_FXAS21002 = new Adafruit_FXAS21002C(0x0021002C);
	bool setup = inst_FXAS21002->begin();

	print_module_label();
	LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(JsonArrayConst p)
	: Loom_FXAS21002( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::~Loom_FXAS21002() 
{
	delete inst_FXAS21002;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "gx: ", gyro[0]);
	LPrintln("\t", "gy: ", gyro[1]);
	LPrintln("\t", "gz: ", gyro[2]);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::measure()
{
	sensors_event_t event;
	inst_FXAS21002->getEvent(&event);

	gyro[0] = event.gyro.x;
	gyro[1] = event.gyro.y;
	gyro[2] = event.gyro.z;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::package(JsonObject json)
{
	package_json(json, module_name, 
		"gx", gyro[0],
		"gy", gyro[1],
		"gz", gyro[2]

	);
}

///////////////////////////////////////////////////////////////////////////////



