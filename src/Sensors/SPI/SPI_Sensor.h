///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SPI_Sensor.h
/// @brief		File for LoomSPISensor definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "../Sensor.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class for SPI sensor modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_p_i_sensor.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomSPISensor : public LoomSensor
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
	LoomSPISensor(
			LoomManager* manager,
			const char* module_name,
			const LoomModule::Type		module_type,
			const uint8_t				num_samples		= 1
		);

	/// Destructor
	virtual ~LoomSPISensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_measurements() const override = 0;

private:

};


