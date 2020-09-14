///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SDI12_Sensor.cpp
/// @brief		File for LoomSDI12Sensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SDI12_Sensor.h"

///////////////////////////////////////////////////////////////////////////////
LoomSDI12Sensor::LoomSDI12Sensor(
		LoomManager* manager,
		const char* module_name,
		const LoomModule::Type		module_type,
		const uint8_t				num_samples 
	) 
	: LoomSensor(manager, module_name, module_type, num_samples ) {}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS