
#include "Loom_I2C_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomI2CSensor::LoomI2CSensor( 	
		const char*		module_name, 
		byte			i2c_address,
		uint8_t			mux_port 
	) 
	: LoomSensor( mux_port ? (String(module_name)+'_'+String(mux_port)).c_str() : module_name )
	, i2c_address(i2c_address)
	, port_num(mux_port)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensor::print_config()
{
	LoomSensor::print_config();
	LPrint("\tI2C Address         : ");
	LPrintln_Dec_Hex(i2c_address);
}

///////////////////////////////////////////////////////////////////////////////
byte LoomI2CSensor::get_i2c_address() 
{ 
	return i2c_address; 
}

///////////////////////////////////////////////////////////////////////////////
