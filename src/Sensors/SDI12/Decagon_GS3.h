///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_GS3.h
/// @brief		File for DecagonGS3 definition. Incomplete.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Decagon GS3 soil moisture sensor module. Incomplete.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class DecagonGS3 : public SDI12Sensor
{

protected:

	float		dielec_perm;	///< Measured dielectric permativity
	float		temp;			///< Measured temperature
	float		elec_cond;		///< Measure electrical conductivity

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	DecagonGS3(
			const uint8_t	num_samples	= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	DecagonGS3(JsonArrayConst p);

	/// Destructor
	~DecagonGS3() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
// Uncomment once implemented
// REGISTER(Module, DecagonGS3, "DecagonGS3");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


