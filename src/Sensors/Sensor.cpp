///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Sensor.cpp
/// @brief		File for LoomSensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Sensor.h"

///////////////////////////////////////////////////////////////////////////////
LoomSensor::LoomSensor(	
		const char*				module_name,
		const LoomModule::Type	module_type,
		const uint8_t			num_samples 
	) 
	: LoomModule(module_name, module_type) 
	, num_samples(num_samples)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomSensor::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tNum Samples         : ", num_samples);
}

///////////////////////////////////////////////////////////////////////////////
