///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Sensor.h
/// @brief		File for LoomSensor definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract root of Loom sensor modules hierarchy
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_sensor.html)
///
///////////////////////////////////////////////////////////////////////////////

class LoomSensor : public LoomModule
{

protected:

	/// How many samples to take and average each call to measure()
	uint8_t		num_samples;	

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	num_samples		The number of samples to take and average
	LoomSensor(
			LoomManager* manager,
			const char* module_name,
			const LoomModule::Type	module_type,
 			const uint8_t			num_samples
		);

	/// Destructor
	virtual ~LoomSensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Calibration routine
	virtual void	calibrate() {};

	/// Take any relevant measurements
	virtual void	measure() = 0;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;

	/// Print the last set of measurements taken.
	/// Does not package the data.
	virtual void	print_measurements() const = 0; 

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the number of samples sensor is set to take
	/// @return		Number of samples
	uint8_t			get_num_samples() const { return num_samples; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the number of samples to take
	/// @param[in]	n	Number of samples to take
	void			set_num_samples(const uint8_t n) { num_samples = n; }

private:

};

