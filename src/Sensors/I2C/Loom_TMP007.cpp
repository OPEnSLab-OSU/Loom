
#include "Loom_TMP007.h"


///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(
		byte			i2c_address, 
		const char*		module_name 
	) 
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = LoomModule::Type::TMP007;

	inst_tmp007 = new Adafruit_TMP007(i2c_address);
	bool setup = inst_tmp007->begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(JsonArrayConst p)
	: Loom_TMP007( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::~Loom_TMP007()
{
	delete inst_tmp007;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Object Temp : ", object_temp);
	LPrintln("\t", "Die Temp    : ", die_temp);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::measure()
{
	object_temp = inst_tmp007->readObjTempC();
	die_temp    = inst_tmp007->readDieTempC();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::package(JsonObject json)
{
	package_json(json, module_name, 
		"objTemp",	object_temp,
		"dieTemp",	die_temp
	);
}

///////////////////////////////////////////////////////////////////////////////
	