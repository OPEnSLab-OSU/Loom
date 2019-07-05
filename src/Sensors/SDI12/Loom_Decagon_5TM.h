#pragma once

#include "Loom_SDI12_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSDI12Sensor) | dependencies: [] | conflicts: []
/// Decagon 5TM soil moisture sensor module
// ###
class Loom_Decagon5TM : public LoomSDI12Sensor
{

protected:


public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon 5TM module constructor
	///
	/// \param[in]	module_name			String | <"5TM"> | null | Decagon 5TM manager module name
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Loom_Decagon5TM(
			const char*		module_name		= "5TM Decagon",
			uint8_t			num_samples		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Decagon5TM(JsonArrayConst p);

	/// Destructor
 	~Loom_Decagon5TM() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_measurements() override;

private:

};



