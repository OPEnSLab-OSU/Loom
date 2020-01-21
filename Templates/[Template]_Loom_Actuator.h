#pragma once

#include "Actuator.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomActuatorTemplate : public LoomActuator
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomActuatorTemplate(
LoomManager* manager,
const char* module_name		= "ActuatorTemplate",
			int 			ex_param		= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomActuatorTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomActuatorTemplate() = default;

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




