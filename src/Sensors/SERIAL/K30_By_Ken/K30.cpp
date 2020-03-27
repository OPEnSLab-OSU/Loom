#include "K30.h"


///////////////////////////////////////////////////////////////////////////////
Loom_K30::Loom_K30(
LoomManager* manager,
const byte i2c_address,
        const uint8_t   mux_port       
	) 
	: LoomI2CSensor(manager, "K30", Type::K30, i2c_address, mux_port )
{
	bool setup = inst_k30.begin(i2c_address);

    if(!setup) active = false;

    print_module_label();
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_K30::Loom_K30(LoomManager* manager, JsonArrayConst p)
	: Loom_K30( EXPAND_ARRAY(p, 2) ) {}


///////////////////////////////////////////////////////////////////////////////
void Loom_K30::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_K30::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void Loom_K30::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////