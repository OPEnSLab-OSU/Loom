///////////////////////////////////////////////////////////////////////////////
///
/// @file		ActuatorTemplate.h
/// @brief		File for ActuatorTemplate definition
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS
#pragma once

#include "Actuator.h"

namespace  {

///////////////////////////////////////////////////////////////////////////////


// ### (Actuator) | dependencies: [] | conflicts: []
/// Class description
// ###
class ActuatorTemplate : public Actuator
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	ActuatorTemplate(
			int 			ex_param		= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	ActuatorTemplate(JsonArrayConst p);

	/// Destructor
	virtual ~ActuatorTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		power_down() override {}
	void		power_up() override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

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
REGISTER(Module, ActuatorTemplate, "ActuatorTemplate");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_ACTUATORS
