///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_I2C_Sensor.cpp
/// @brief		File for LoomI2CSensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "I2C_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomI2CSensor::LoomI2CSensor(
		LoomManager* manager,
		const char* module_name, 
		const LoomModule::Type		module_type,
		const byte					i2c_address,
		const uint8_t				mux_port,
		const uint8_t				num_samples
	) 
	: LoomSensor(manager, (mux_port != 255)  ? (String(module_name)+'_'+String(mux_port)).c_str() : module_name, module_type, num_samples )
	, i2c_address(i2c_address)
	, port_num(mux_port)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensor::print_config() const
{
	LoomSensor::print_config();
	LPrint("\tI2C Address         : ");
	LPrintln_Dec_Hex(i2c_address);
}

///////////////////////////////////////////////////////////////////////////////
