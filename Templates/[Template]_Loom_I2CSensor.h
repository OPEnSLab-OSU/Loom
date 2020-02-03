#pragma once

#include "I2C_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomI2CSensorTemplate : public LoomI2CSensor
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomI2CSensorTemplate(
			byte			i2c_address		= 0x2A,
			LoomManager* manager,
const char* module_name		= "I2CSensorTemplate"
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomI2CSensorTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomI2CSensorTemplate() = default;

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




