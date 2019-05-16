
#include "Loom_SHT31D.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_SHT31D::Loom_SHT31D(
		byte			i2c_address,
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::SHT31D;

	bool setup = inst_sht31d.begin(i2c_address);

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_SHT31D::Loom_SHT31D(JsonVariant p)
	: Loom_SHT31D(p[0], p[1]) 
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_SHT31D::~Loom_SHT31D() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Temperature: ", temp, " C");
	LPrintln("\t", "Humidity   : ", humid);
}

/////////////////////////////////////////////////////////////////////
void Loom_SHT31D::measure()
{
	float t = inst_sht31d.readTemperature();
	float h = inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		temp = t;
		humid = h;
	} else {
		print_module_label();
		LPrintln("Failed to read temperature or humidity");
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_SHT31D::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Temp", temp, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "Humid", humid);
}


/////////////////////////////////////////////////////////////////////

void Loom_SHT31D::package(JsonObject json)
{
	package_json(json, module_name, 
		"Temp",		temp,
		"Humid",	humid
	);
}

/////////////////////////////////////////////////////////////////////

