#pragma once

#include "Loom_SDI12_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSDI12Sensor) | dependencies: [] | conflicts: []
/// Decagon GS3 Soil Moisture sensor module
// ###
class Loom_DecagonGS3 : public LoomSDI12Sensor
{

protected:

	float		dielec_perm;	/// Measured dielectric permativity
	float		temp;			/// Measured temperature
	float		elec_cond;		/// Measure electrical conductivity

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// \param[in]	module_name			String | <"GS3"> | null | Decagon GS3 manager module name
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Loom_DecagonGS3(
			const char*		module_name		= "GS3 Decagon",
			uint8_t			num_samples		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_DecagonGS3(JsonVariant p);

	/// Destructor
	~Loom_DecagonGS3() = default;

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


