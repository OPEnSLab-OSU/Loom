
#include "Loom_SHT31D.h"



// --- CONSTRUCTOR ---
Loom_SHT31D::Loom_SHT31D(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	bool setup = inst_sht31d.begin(i2c_address);

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_SHT31D::~Loom_SHT31D() 
{

}



void Loom_SHT31D::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println4("\t", "Temperature: ", temp, " C");
	LOOM_DEBUG_Println3("\t", "Humidity   : ", humid);
}


void Loom_SHT31D::measure()
{
	float t = inst_sht31d.readTemperature();
	float h = inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		temp = t;
		humid = h;
	} else {
		print_module_label();
		LOOM_DEBUG_Println("Failed to read temperature or humidity");
	}
}


void Loom_SHT31D::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	append_to_bundle_key_value(bndl, id_prefix, "Temp", temp);
	append_to_bundle_key_value(bndl, id_prefix, "Humid", humid);
}


void Loom_SHT31D::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_msg_key_value(bndl, "Temp", temp);
	append_to_bundle_msg_key_value(bndl, "Humid", humid);
}
