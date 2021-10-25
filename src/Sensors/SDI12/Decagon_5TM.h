///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_5TM.h
/// @brief		File for Decagon5TM definition. Incomplete.
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
/// Decagon 5TM soil moisture sensor module. Incomplete.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon5_t_m.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class Decagon5TM : public SDI12Sensor
{
protected:

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon 5TM module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Decagon5TM(
			const uint8_t 	sdiPin = 11,
			const uint8_t	num_samples	= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Decagon5TM(JsonArrayConst p);

	/// Destructor
 	~Decagon5TM() = default;

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
// REGISTER(Module, Decagon5TM, "Decagon5TM");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

