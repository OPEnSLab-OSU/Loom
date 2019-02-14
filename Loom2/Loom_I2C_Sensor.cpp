
#include "Loom_I2C_Sensor.h"
// #include "Loom_Macros.h"



// --- CONSTRUCTOR ---
LoomI2CSensor::LoomI2CSensor( 	char* module_name, 
			char* sensor_description, 
			byte  i2c_address 
		) : LoomSensor( module_name, sensor_description )
{
	this->i2c_address = i2c_address;
}



// --- DESTRUCTOR ---
LoomI2CSensor::~LoomI2CSensor() 
{

}

// --- PUBLIC METHODS ---

void LoomI2CSensor::print_config()
{
	LoomSensor::print_config();
	LOOM_DEBUG_Print2('\t', "I2C Address         : ");
	LOOM_DEBUG_Println_Dec_Hex(i2c_address);
}


	// Get the sensor's I2C address
byte LoomI2CSensor::get_i2c_address() 
{ 
	return i2c_address; 
}


// void LoomI2CSensor::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s/%d", id_prefix, module_name, port);
// 	bndl->add(address_string);
// }
