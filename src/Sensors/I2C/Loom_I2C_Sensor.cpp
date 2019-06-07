
#include "Loom_I2C_Sensor.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomI2CSensor::LoomI2CSensor( 	
		const char*		module_name, 
		byte			i2c_address 
	) 
	: LoomSensor( module_name )
{
	this->i2c_address = i2c_address;
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomI2CSensor::~LoomI2CSensor() 
{

}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomI2CSensor::print_config()
{
	LoomSensor::print_config();
	LPrint('\t', "I2C Address         : ");
	LPrintln_Dec_Hex(i2c_address);
}

/////////////////////////////////////////////////////////////////////
byte LoomI2CSensor::get_i2c_address() 
{ 
	return i2c_address; 
}

/////////////////////////////////////////////////////////////////////
void LoomI2CSensor::adjust_module_name_with_port(int port)
{
	snprintf(module_name, 20, "%s_%d", module_name, port); 
}

/////////////////////////////////////////////////////////////////////
