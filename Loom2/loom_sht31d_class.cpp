

#include "loom_sht31d_class.h"

#include "loom_macros.h"

#include <Wire.h>
// #include "Adafruit_SHT31.h"




// --- CONSTRUCTOR ---
Loom_SHT31D::Loom_SHT31D(char* module_name, char* sensor_description, byte i2c_address)
	: I2CSensor( module_name, sensor_description, i2c_address )
{
	LOOM_DEBUG_Println("SHT31D Setup");

	bool setup = inst_sht31d.begin(i2c_address);

	LOOM_DEBUG_Println4("\tInitialize ", module_name, " ", (setup) ? "sucessful" : "failed");
}

// maybe a constructor that specifies the i2c address (use a default otherwise)

// --- DESTRUCTOR ---
Loom_SHT31D::~Loom_SHT31D() 
{

}


// --- PUBLIC METHODS ---

// void Loom_SHT31D::print_config()
// {
// 	I2CSensor::print_config();
// }


void Loom_SHT31D::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println2("\tTemperature: ", temp);
	LOOM_DEBUG_Println2("\tHumidity   : ", humid);
}

void Loom_SHT31D::measure()
{
	float t = inst_sht31d.readTemperature();
	float h = inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		temp = t;
		humid = h;
	} else {
		LOOM_DEBUG_Println("Failed to read temperature or humidity");
	}
}

void Loom_SHT31D::package(OSCBundle* bndl, char* id_prefix)
{

}

