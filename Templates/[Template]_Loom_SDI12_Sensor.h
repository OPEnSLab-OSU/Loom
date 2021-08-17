///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12SensorTemplate.h
/// @brief		File for SDI12SensorTemplate definition.
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////


// ### (SDI12Sensor) | dependencies: [] | conflicts: []
/// Class description
// ###
class SDI12SensorTemplate : public SDI12Sensor
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	SDI12SensorTemplate(
const char* module_name		= "SDI12SensorTemplate",
			int				num_samples		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	SDI12SensorTemplate(JsonArrayConst p);

	/// Destructor
	virtual ~SDI12SensorTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		calibrate() override;
	void		power_down() override {}
	void		power_up() override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;
	void		print_measurements() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



private:

	// Private members (are not inherited)

	// Auxiliary functions

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, SDI12SensorTemplate, "SDI12SensorTemplate");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
