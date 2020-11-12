///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SDI12_Sensor.h
/// @brief		File for LoomSDI12Sensor definition. Incomplete.
/// @author		
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "../Sensor.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class for SDI12 sensor modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_d_i12_sensor.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class LoomSDI12Sensor : public LoomSensor
{

protected:


public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	num_samples		The number of samples to take and average
	LoomSDI12Sensor(
			LoomManager* manager,
			const char* module_name,
			const LoomModule::Type		module_type,
			const uint8_t				num_samples = 1
		);

	/// Destructor
	virtual ~LoomSDI12Sensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================


private:

};


