///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_I2C_Sensor.cpp
/// @brief		File for I2CSensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "I2C_Sensor.h"

///////////////////////////////////////////////////////////////////////////////
I2CSensor::I2CSensor(
		const char*				module_name, 
		const LoomModule::Type	module_type,
		const byte				i2c_address,
		const uint8_t			mux_port,
		const uint8_t			num_samples
	) 
	: Sensor((mux_port != 255)  ? (String(module_name)+'_'+String(mux_port)).c_str() : module_name, module_type, num_samples)
	, i2c_address(i2c_address)
	, port_num(mux_port)
{}

///////////////////////////////////////////////////////////////////////////////
void I2CSensor::print_config() const
{
	Sensor::print_config();
	LPrint("\tI2C Address         : ");
	LPrintln_Dec_Hex(i2c_address);
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
