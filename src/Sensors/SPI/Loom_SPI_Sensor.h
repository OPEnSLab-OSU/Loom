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

#include "../Loom_Sensor.h"


/// Abstract base class for SPI sensor modules 
class LoomSPISensor : public LoomSensor
{

protected:


public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// \param[in]	module_name		Name of the module (provided by derived classes)
	/// \param[in]	module_type		Type of the module (provided by derived classes)
	/// \param[in]	num_samples		The number of samples to take and average
	LoomSPISensor(	
			const char*			module_name,
			LoomModule::Type	module_type,
			uint8_t				num_samples		= 1
		);

	/// Destructor
	virtual ~LoomSPISensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_measurements() override = 0;

private:

};


