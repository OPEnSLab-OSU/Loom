#pragma once

#include "SDI12_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSDI12Sensor) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomSDI12SensorTemplate : public LoomSDI12Sensor
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomSDI12SensorTemplate(
			LoomManager* manager,
const char* module_name		= "SDI12SensorTemplate",
			int				num_samples		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomSDI12SensorTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomSDI12SensorTemplate() = default;

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




