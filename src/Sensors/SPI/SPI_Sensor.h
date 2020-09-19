///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SPI_Sensor.h
/// @brief		File for SPISensor definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "../Sensor.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class for SPI sensor modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_p_i_sensor.html)
///
///////////////////////////////////////////////////////////////////////////////
class SPISensor : public Sensor
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
	SPISensor(
			const char*				module_name,
			const LoomModule::Type	module_type,
			const uint8_t			num_samples	= 1
		);

	/// Destructor
	virtual ~SPISensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_measurements() const override = 0;

private:

};

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

