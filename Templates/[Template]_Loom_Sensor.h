#pragma once

#include "Loom_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSensor) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomSensorTemplate : public LoomSensor
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomSensorTemplate(
			const char*		module_name		= "SensorTemplate",
			int 			num_samples		= 4
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomSensorTemplate(JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomSensorTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	bool		cmd_route(JsonObject json) override;
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




